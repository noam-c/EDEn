begin
  require 'rubygems' 
rescue LoadError
end

require 'FrameFinder'
require 'wx'

include Wx

DIALOGS_FILE_OPEN_IMAGE = 10
DIALOGS_FILE_OPEN_SHEET = 11
DIALOGS_FILE_SAVE = 12

class SpritesheetEditorFrame < Frame
  def initialize()
    super(nil, -1, 'Spritesheet Editor')
    @offset = 20
    height = 450 + (2 * @offset)
    width = 500 + (2 * @offset)
    self.client_size = [width, height]

    @prevButton = Button.new(self, :label => 'Previous')
    @nextButton = Button.new(self, :label => 'Next')
    @renameButton = Button.new(self, :label => 'Rename')
    @frameNameBox = TextCtrl.new(self, -1)
    @diagnostics = StaticText.new(self, -1, 'Diagnostics')
    @framesListBox = ListBox.new(self, :size => [-1,height - 100], :style => LB_SINGLE | LB_NEEDED_SB)

    mainSizer = VBoxSizer.new
    frameSizer = HBoxSizer.new

    buttonsSizer = HBoxSizer.new
    buttonsSizer.add(@prevButton, 0, 0, 0)
    buttonsSizer.add(@nextButton, 0, 0, 0)

    leftSideSizer = VBoxSizer.new
    leftSideSizer.add(@framesListBox, 1, EXPAND, 0)
    leftSideSizer.add(buttonsSizer, 1, EXPAND | TOP, 5)

    frameNameSizer = HBoxSizer.new
    frameNameSizer.add(@frameNameBox, 3, ALL, 2)
    frameNameSizer.add(@renameButton, 1, ALL, 2)

    rightSideSizer = VBoxSizer.new
    rightSideSizer.add_stretch_spacer(2)
    rightSideSizer.add(frameNameSizer, 1, EXPAND | TOP, 5)
    @spriteRectangle = rightSideSizer.add(0, 0, 6, EXPAND, 0)
    rightSideSizer.add_stretch_spacer(2)
    
    frameSizer.add(leftSideSizer, 0, TOP | LEFT | RIGHT, @offset)
    frameSizer.add(rightSideSizer, 0, TOP | LEFT | RIGHT, @offset)

    mainSizer.add(frameSizer)
    mainSizer.add(@diagnostics, 0, ALIGN_BOTTOM | EXPAND | ALL, 5)
    self.sizer = mainSizer
    
    evt_button(@renameButton.get_id()) { set_frame_name() }
    evt_button(@prevButton.get_id()) { prev_frame() }
    evt_button(@nextButton.get_id()) { next_frame() }
    
    evt_menu(DIALOGS_FILE_OPEN_IMAGE) { on_file_open_image() }
    evt_menu(DIALOGS_FILE_OPEN_SHEET) { on_file_open_sheet() }
    evt_menu(DIALOGS_FILE_SAVE) { on_file_save() }

    evt_listbox(@framesListBox.get_id()) { |event| on_list_click(event) }

    createMenubar()

    @background_brush = Brush.new('pink', CROSS_HATCH)

    @bitmap = nil

    @frameNum = 0

    # Set up the drawing to be done when the frame needs re-painting
    evt_paint :on_paint
  end

  def on_list_click(event)
    set_frame_num(event.get_selection())
  end

  def createMenubar()
    file_menu = Menu.new
    file_menu.append(DIALOGS_FILE_OPEN_IMAGE,  "Open &image...\tCtrl-I")
    file_menu.append(DIALOGS_FILE_OPEN_SHEET,  "&Open spritesheet...\tCtrl-O")
    file_menu.append(DIALOGS_FILE_SAVE,  "&Save spritesheet\tCtrl-S")
    file_menu.append_separator()
    file_menu.append(ID_EXIT, "E&xit\tAlt-X")

    menu_bar = MenuBar.new
    menu_bar.append(file_menu, "&File")
    set_menu_bar(menu_bar)
  end

  def on_file_open_image()
    dialog = FileDialog.new(
                             self,
                             "Open Image...",
                             "",
                             "",
                             "PNG files (*.png)|*.png",
                             FD_OPEN
                           )

    dialog.set_directory(get_home_dir())

    if dialog.show_modal() == ID_OK then
      @imagePath = dialog.get_path()
      @dataPath = @imagePath.sub(/\.png/, "\.eds")

      @spritesheet = Image.new(@imagePath)
      @frames = FrameFinder.new(@spritesheet)
      populate_frame_list()
    end
  end

  def on_file_open_sheet()
    dialog = FileDialog.new(
                             self,
                             "Open Spritesheet...",
                             "",
                             "",
                             "ED Spritesheet files (*.eds)|*.eds",
                             FD_OPEN
                           )

    dialog.set_directory(get_home_dir())

    if dialog.show_modal() == ID_OK then
      @dataPath = dialog.get_path()
      @imagePath = @dataPath.sub(/\.eds/, "\.png")

      @spritesheet = Image.new(@imagePath)
      @frames = FrameFinder.new(@dataPath)
      populate_frame_list()
    end
  end

  def on_file_save()
    if @frames != nil
      @frames.save_to_file(@dataPath)
    end
  end

  def populate_frame_list()
      return if @frames == nil
      numFrames = @frames.get_num_frames()
      frameNameList = Array.new(numFrames) {|i| @frames.get_frame(i).name}

      @framesListBox.set(frameNameList)
      set_frame_num(0)
  end

  def set_frame_num(num)
    return if @frames == nil
    
    if num < 0
      num = 0
    end

    if num >= @frames.get_num_frames
      num = @frames.get_num_frames - 1
    end

    @frameNum = num
    @frame = @frames.get_frame(num)
    @frameNameBox.change_value(@frame.name)

    @framesListBox.set_selection(@frameNum)

    @bitmap = @spritesheet.get_sub_image(Rect.new(@frame.leftEdge, @frame.topEdge, @frame.rightEdge - @frame.leftEdge, @frame.bottomEdge - @frame.topEdge)).to_bitmap
    
    @diagnostics.set_label("Frame: " + @frame.leftEdge.to_s + "," + @frame.topEdge.to_s + "," + @frame.rightEdge.to_s + "," + @frame.bottomEdge.to_s)
    refresh
    update
  end

  def set_frame_name()
    @frame.name = @frameNameBox.get_value
    @framesListBox.set_string(@frameNum, @frame.name)
  end

  def next_frame()
    set_frame_num(@frameNum + 1)
  end

  def prev_frame()
    set_frame_num(@frameNum - 1)
  end

  def on_paint
    paint do | dc |
      rect = @spriteRectangle.get_rect()

      dc.set_brush(Wx::TRANSPARENT_BRUSH)

      # Draw a 1-pixel border around the GUI area reserved for the bitmap
      draw_border(dc, rect)

      if @bitmap != nil then
        rect.width = @bitmap.get_width()
        rect.height = @bitmap.get_height()
  
        dc.set_brush(@background_brush)

        # Draw a 1-pixel border around the GUI area reserved for the bitmap
        draw_border(dc, rect)

        # Draw the bitmap at the specified point with transparency
        dc.draw_bitmap(@bitmap, rect.x, rect.y, true)
      end
    end
  end
  
  def draw_border(dc, rect)
    dc.draw_rectangle(rect.x - 1, rect.y - 1, rect.width + 2, rect.height + 2)
  end
end

Wx::App.run do
  frame = SpritesheetEditorFrame.new
  frame.show
end