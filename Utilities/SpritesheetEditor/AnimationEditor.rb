require "set"
begin
  require 'rubygems' 
rescue LoadError
end

require 'FrameFinder'
require 'wx'

include Wx

DIALOGS_FILE_OPEN_SHEET = 11
DIALOGS_FILE_SAVE = 12

class AnimationEditorFrame < Frame
  def initialize()
    super(nil, -1, 'Spritesheet Editor')
    @offset = 20
    height = 600 + (2 * @offset)
    width = 600 + (2 * @offset)
    self.client_size = [width, height]

    @animNameBox = TextCtrl.new(self, -1)
    @renameButton = Button.new(self, :label => 'Rename')
    @appendButton = Button.new(self, :label => 'Append')
    @insertButton = Button.new(self, :label => 'Insert')
    @removeButton = Button.new(self, :label => 'Remove')
    @leftShiftButton = Button.new(self, :label => '<<')
    @rightShiftButton = Button.new(self, :label => '>>')

    @previewFrameList = ListBox.new self
    
    @animListBox = ListBox.new(self, :style => LB_SINGLE | LB_NEEDED_SB)
    @animFrameList = ListCtrl.new(self, -1)
    @newAnimationButton = Button.new(self, :label => 'New Animation')

    @diagnostics = StaticText.new(self, -1, 'Diagnostics')

    addFrameSizer = HBoxSizer.new
    addFrameSizer.add(@appendButton, 0, EXPAND, 0)
    addFrameSizer.add(@insertButton, 0, EXPAND, 0)
    
    leftSideSizer = VBoxSizer.new
    leftSideSizer.add(@previewFrameList, 0, EXPAND, 0)
    leftSideSizer.add(addFrameSizer, 0, EXPAND, 0)
    
    rightSideSizer = VBoxSizer.new
    @framePreviewRect = rightSideSizer.add(150, 150, 0, 0, 0)
    
    animSizer = HBoxSizer.new
    animSizer.add(leftSideSizer, 1, TOP | LEFT | RIGHT, @offset)
    animSizer.add(rightSideSizer, 2, TOP | LEFT | RIGHT, @offset)

    animNameSizer = HBoxSizer.new
    animNameSizer.add(@animListBox, 2, EXPAND, 0)
    animNameSizer.add(@animNameBox, 3, ALIGN_TOP | ALL, 2)
    animNameSizer.add(@renameButton, 1, ALIGN_TOP | ALL, 2)

    headerSizer = VBoxSizer.new
    headerSizer.add(animNameSizer, 0, 0, 0)
    headerSizer.add(@newAnimationButton, 0, 0, 0)

    animEditSizer = HBoxSizer.new
    animEditSizer.add(@leftShiftButton, 0, 0, 0)
    animEditSizer.add(@removeButton, 0, 0, 0)
    animEditSizer.add(@rightShiftButton, 0, 0, 0)
    
    mainSizer = VBoxSizer.new
    mainSizer.add(headerSizer, 0, ALL, @offset)
    mainSizer.add(animSizer)
    mainSizer.add(@animFrameList, 0, EXPAND | TOP | LEFT | RIGHT, @offset)
    mainSizer.add(animEditSizer, 0, ALIGN_RIGHT | BOTTOM | LEFT | RIGHT, @offset)
    @animPreviewRect = mainSizer.add(150, 150, 0, ALIGN_CENTER_HORIZONTAL | BOTTOM | LEFT | RIGHT, @offset)
    mainSizer.add(@diagnostics, 0, ALIGN_BOTTOM | EXPAND | ALL, 5)
    self.sizer = mainSizer
    
    evt_button(@newAnimationButton.get_id()) { add_new_anim() }
    evt_button(@renameButton.get_id()) { set_animation_name }
    evt_button(@appendButton.get_id()) { append_frame }
    evt_button(@insertButton.get_id()) { insert_frame }
    evt_button(@removeButton.get_id()) { remove_frame }
    evt_button(@leftShiftButton.get_id()) { shift_frame_left }
    evt_button(@rightShiftButton.get_id()) { shift_frame_right }

    evt_menu(DIALOGS_FILE_OPEN_SHEET) { on_file_open_sheet() }
    evt_menu(DIALOGS_FILE_SAVE) { on_file_save() }

    evt_listbox(@previewFrameList.get_id()) { |event| on_preview_list_click(event) }
    evt_listbox(@animListBox.get_id()) { |event| on_anim_list_click(event) }

    createMenubar()

    @background_brush = Brush.new('pink', CROSS_HATCH)

    @animNum = 0
    @animFrame = 0

    @previewFrame = 0

    @previewFrameBitmap = nil

    # Set up the drawing to be done when the frame needs re-painting
    evt_paint :on_paint
    
    # Set up the animation cycling timer
    @animTimer = Timer.every(200) do
      if @animation != nil && @animation.get_num_frames > 0
        @animFrame = (@animFrame + 1) % @animation.get_num_frames
        refresh
      end
    end
  end

  def on_preview_list_click(event)
    set_frame_num event.get_selection
  end

  def on_anim_list_click(event)
    set_anim_num event.get_selection
  end

  def createMenubar()
    file_menu = Menu.new
    file_menu.append(DIALOGS_FILE_OPEN_SHEET,  "&Open spritesheet...\tCtrl-O")
    file_menu.append(DIALOGS_FILE_SAVE,  "&Save spritesheet\tCtrl-S")
    file_menu.append_separator()
    file_menu.append(ID_EXIT, "E&xit\tAlt-X")

    menu_bar = MenuBar.new
    menu_bar.append(file_menu, "&File")
    set_menu_bar(menu_bar)
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
      @model = FrameFinder.new @dataPath
      populate_frame_list
      populate_anim_list
    end
  end

  def on_file_save()
    if @model != nil
      @model.save_to_file(@dataPath)
    end
  end

  def add_new_anim()
    return if @model == nil
    index = @model.add_animation()
    @animListBox.insert_items([@model.get_animation(index).name], index)
    @animListBox.set_selection(index)
    set_anim_num(index)
  end

  def append_frame
    insert_frames_into(-1)
  end

  def insert_frame
    insertion_points = @animFrameList.get_selections()
    return if insertion_points.length != 1

    insert_frames_into(insertion_points[0] + 1)    
  end

  def insert_frames_into(pos)
    return if @animation == nil
    selections = @previewFrameList.get_selections()
    selected_frames = Array.new(selections.length) {|i| @model.get_frame(selections[i])}
    @animation.insert(pos, selected_frames)
    update_animation
  end

  def remove_frame
    return if @model == nil || @animation == nil
    @animation.remove(@animFrameList.get_selections())
    update_animation
  end

  def shift_frame_left
    return if @model == nil || @animation == nil
    selected = @animFrameList.get_selections()
    return if selected.length != 1 || selected[0] == 0
    @animation.swap(selected[0], selected[0] - 1)
    update_animation
  end

  def shift_frame_right
    return if @model == nil || @animation == nil
    selected = @animFrameList.get_selections()
    return if selected.length != 1 || selected[0] > @animation.get_num_frames() - 2
    @animation.swap(selected[0], selected[0] + 1)
    update_animation
  end
  
  def populate_frame_list()
    return if @model == nil
    numFrames = @model.get_num_frames
    frameNameList = Array.new()

    for i in (0..numFrames - 1) do
      name = @model.get_frame(i).name

      if name != UNTITLED_LINE
        frameNameList << name
      end
    end

    @previewFrameList.set(frameNameList)
    set_frame_num(0)
    
    animFrameImages = ImageList.new(75, 75)
    (0..@model.get_num_frames() - 1).each do |i|
      currFrame = @model.get_frame(i)
      next if currFrame.name == UNTITLED_LINE
      width = currFrame.rightEdge - currFrame.leftEdge
      height = currFrame.bottomEdge - currFrame.topEdge
      image = @spritesheet.get_sub_image(Rect.new(currFrame.leftEdge, currFrame.topEdge, width, height))
      image.rescale(75, 75)
      animFrameImages.add(image.to_bitmap)
    end

    @animFrameList.set_image_list(animFrameImages, IMAGE_LIST_NORMAL)
  end

  def populate_anim_list()
    return if @model == nil
    numAnimations = @model.get_num_animations
    animNameList = Array.new(numAnimations) {|i| @model.get_animation(i).name}

    @animListBox.set(animNameList)
    set_anim_num(0)
  end

  def set_frame_num(num)
    return if @model== nil
    
    if num < 0
      num = 0
    end

    if num >= @model.get_num_frames
      num = @model.get_num_frames - 1
    end

    @previewFrameList.set_selection(num)
    previewFrameName = @previewFrameList.get_string(num)
    @previewFrame = @model.get_frame(previewFrameName)
    rect = Rect.new(@previewFrame.leftEdge,
                    @previewFrame.topEdge,
                    @previewFrame.rightEdge - @previewFrame.leftEdge,
                    @previewFrame.bottomEdge - @previewFrame.topEdge)

    @previewFrameBitmap = @spritesheet.get_sub_image(rect).to_bitmap

    refresh
    update
  end

  def set_anim_num(num)
    return if @model == nil || @model.get_num_animations == 0
    
    if num < 0
      num = 0
    end

    if num >= @model.get_num_animations
      num = @model.get_num_animations - 1
    end

    @animNum = num
    @animFrame = 0
    @animation = @model.get_animation(num)
    @animNameBox.change_value(@animation.name)

    update_animation
  
    @animListBox.set_selection(@animNum) 
        
    @diagnostics.set_label("Animation: " + @animation.name)
    refresh
    update
  end

  def update_animation
    # Reset the frame for the animation preview
    @animFrame = 0 

    # Update the animation frame list
    @animFrameList.delete_all_items()
    (0..@animation.get_num_frames - 1).each do |i|
      currFrame = @animation.get_frame(i)
      @animFrameList.insert_item(i, currFrame.name, currFrame.index)
    end
  end

  def set_animation_name()
    @animation.name = @animNameBox.get_value
    @animListBox.set_string(@animNum, @animation.name)
  end

  def on_paint
    paint do | dc |
      animRect = @animPreviewRect.get_rect()
      frameRect = @framePreviewRect.get_rect()

      dc.set_brush(Wx::TRANSPARENT_BRUSH)

      # Draw a 1-pixel border around the GUI area reserved for the frame preview
      draw_border(dc, frameRect)

      # Draw a 1-pixel border around the GUI area reserved for the animation preview
      draw_border(dc, animRect)

      if @previewFrameBitmap != nil
        frameRect.width = @previewFrameBitmap.get_width()
        frameRect.height = @previewFrameBitmap.get_height()
  
        dc.set_brush(@background_brush)

        # Draw a 1-pixel border around the GUI area reserved for the bitmap
        draw_border(dc, frameRect)

        # Draw the bitmap at the specified point with transparency
        dc.draw_bitmap(@previewFrameBitmap, frameRect.x, frameRect.y, true)
      end

      if @animation != nil && @animation.get_num_frames > 0 
        currFrame = @animation.get_frame(@animFrame)
        bitmap = @spritesheet.get_sub_image(Rect.new(currFrame.leftEdge, currFrame.topEdge, currFrame.rightEdge - currFrame.leftEdge, currFrame.bottomEdge - currFrame.topEdge)).to_bitmap
      
        animRect.width = bitmap.get_width()
        animRect.height = bitmap.get_height()
  
        dc.set_brush(@background_brush)

        # Draw a 1-pixel border around the GUI area reserved for the bitmap
        draw_border(dc, animRect)

        # Draw the bitmap at the specified point with transparency
        dc.draw_bitmap(bitmap, animRect.x, animRect.y, true)
      end
    end
  end

  def draw_border(dc, rect)
    dc.draw_rectangle(rect.x - 1, rect.y - 1, rect.width + 2, rect.height + 2)
  end
end

Wx::App.run do
  frame = AnimationEditorFrame.new
  frame.show
end