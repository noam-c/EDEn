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
    size = 512 + (2 * @offset)
    self.client_size = [size, size]

    sizer = BoxSizer.new(Wx::VERTICAL)
    
    @prevButton = Button.new(self, -1, 'Previous')
    @nextButton = Button.new(self, -1, 'Next')
    @frameNameBox = TextCtrl.new(self, -1)
    @diagnostics = StaticText.new(self, -1, 'Diagnostics')

    sizer.add(@prevButton, 0, Wx::ALIGN_RIGHT, 2)
    sizer.add(@nextButton, 0, Wx::ALIGN_RIGHT, 2)
    sizer.add(@frameNameBox, 0, Wx::ALIGN_RIGHT, 2)
    sizer.add(@diagnostics, 0, Wx::ALIGN_RIGHT, 2)
    
    self.sizer = sizer
    
    evt_text(@frameNameBox.get_id()) { set_frame_name() }
    
    evt_button(@prevButton.get_id()) { prev_frame()}
    evt_button(@nextButton.get_id()) { next_frame()}
    
    evt_menu(DIALOGS_FILE_OPEN_IMAGE) { on_file_open_image() }
    evt_menu(DIALOGS_FILE_OPEN_SHEET) { on_file_open_sheet() }
    evt_menu(DIALOGS_FILE_SAVE) { on_file_save() }

    createMenubar()

    @bitmap = nil

    @frameNum = 0

    # Set up the drawing to be done when the frame needs re-painting
    evt_paint :on_paint
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
      set_frame_num(0)
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
      set_frame_num(0)
    end
  end

  def on_file_save()
    if @frames != nil
      @frames.save_to_file(@dataPath)
    end
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

    @bitmap = @spritesheet.get_sub_image(Rect.new(@frame.leftEdge, @frame.topEdge, @frame.rightEdge - @frame.leftEdge, @frame.bottomEdge - @frame.topEdge)).to_bitmap
    
    @diagnostics.set_label("Frame: " + @frame.leftEdge.to_s + "," + @frame.topEdge.to_s + "," + @frame.rightEdge.to_s + "," + @frame.bottomEdge.to_s)
    refresh
    update
  end

  def set_frame_name()
    @frame.name = @frameNameBox.get_value
  end

  def next_frame()
    set_frame_num(@frameNum + 1)
  end

  def prev_frame()
    set_frame_num(@frameNum - 1)
  end

  def on_paint
    paint do | dc |
      # Draw the bitmap at the specified offset with no transparency
      if @bitmap != nil then
        dc.draw_bitmap(@bitmap, @offset, @offset, false)
      end
    end
  end
end

Wx::App.run do
  frame = SpritesheetEditorFrame.new
  frame.show
end