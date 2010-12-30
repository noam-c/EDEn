begin
  require 'rubygems' 
rescue LoadError
end
require 'wx'

include Wx

# NOTE: If this value must be changed, the UNTITLED_LINE string in the game's Spritesheet class must also change to be in sync 
# As well, any .eds files with the old value must be updated to use the new value instead
UNTITLED_LINE = "untitled"

class PixelPair
  attr_reader :x
  attr_reader :y
  
  def initialize(x, y)
    @x = x
    @y = y
    @strHash = (x.to_s + ',' + y.to_s).hash
  end

  def eql?(other)
    other.is_a?(PixelPair) && x == other.x && y == other.y
  end

  def hash()
    @strHash
  end
end

class SpriteFrame  
  attr_accessor :name

  attr_reader :index

  attr_reader :leftEdge
  attr_reader :topEdge
  attr_reader :rightEdge
  attr_reader :bottomEdge

  attr_reader :pixels

  def initialize(*args)
    
    if args.size == 0
      frameName = UNTITLED_LINE
      leftEdge = -1
      topEdge = -1
      rightEdge = -1
      bottomEdge = -1
    elsif args.size == 6
      frameName = args[0]
      frameIndex = args[1] 
      leftEdge = args[2]
      topEdge = args[3]
      rightEdge = args[4]
      bottomEdge = args[5]
    end

    @name = frameName
    @index = frameIndex

    @leftEdge = leftEdge
    @topEdge = topEdge
    @rightEdge = rightEdge
    @bottomEdge = bottomEdge

    @pixels = []
  end

  def to_s()
    "frame " << @name << ' ' << @leftEdge.to_s << ' ' << @topEdge.to_s << ' ' << @rightEdge.to_s << ' ' << @bottomEdge.to_s << "\n"
  end
  
  def merge(otherFrame)
    @pixels |= otherFrame.pixels
    
    if @leftEdge == -1
      @leftEdge = otherFrame.leftEdge
    elsif otherFrame.leftEdge != -1 && otherFrame.leftEdge < @leftEdge
      @leftEdge = otherFrame.leftEdge
    end

    if @rightEdge == -1
      @rightEdge = otherFrame.rightEdge
    elsif otherFrame.rightEdge != -1 && otherFrame.rightEdge > @rightEdge
      @rightEdge = otherFrame.rightEdge
    end

    if @topEdge == -1
      @topEdge = otherFrame.topEdge
    elsif otherFrame.topEdge != -1 && otherFrame.topEdge < @topEdge
      @topEdge = otherFrame.topEdge
    end

    if @bottomEdge == -1
      @bottomEdge = otherFrame.bottomEdge
    elsif otherFrame.bottomEdge != -1 && otherFrame.bottomEdge > @bottomEdge
      @bottomEdge = otherFrame.bottomEdge
    end
  end
  
  def insert(pixel)
    @pixels << pixel
    x = pixel.x
    y = pixel.y

    if @leftEdge == -1 || x < @leftEdge
      @leftEdge = x
    end
    
    if @rightEdge == -1 || x > @rightEdge
      @rightEdge = x
    end

    if @topEdge == -1 || y < @topEdge
      @topEdge = y
    end
    
    if @bottomEdge == -1 || y > @bottomEdge
      @bottomEdge = y
    end
  end
end

class SpriteAnimation  
  attr_accessor :name

  def initialize(*args)
    @name = args[0]
    @frameList = args[1]
  end

  def get_frame(num)
    return @frameList[num]
  end
  
  def get_num_frames()
    return @frameList.length
  end
  
  def insert(position, frames)
    @frameList.insert(position, frames)
    @frameList.flatten!
  end
  
  def remove(positions)
    positions.sort.reverse.each { |position| @frameList.delete_at(position) }
  end
  
  def swap(pos1, pos2)
    return if pos1 < 0 || pos1 >= @frameList.length
    return if pos2 < 0 || pos2 >= @frameList.length

    temp = @frameList[pos1]
    @frameList[pos1] = @frameList[pos2]
    @frameList[pos2] = temp
  end
  
  def to_s()
    s = "anim " << @name
    @frameList.each { |frame| s << ' ' << frame.name }
    s << "\n"
  end    
end

class FrameFinder
  def initialize(arg)
    @animations = []
    @frameMap = {}

    if arg.is_a?(String)
      load_from_data(arg)
    elsif arg.is_a?(Image)
      load_from_image(arg)
    end
  end

  def load_from_data(filename)
    file = File.new(filename, "r+")
    @frames = []

    while (line = file.gets)
      tokens = line.split(' ')
      kind = tokens[0]
      name = tokens[1]
      if kind == "frame"
        leftEdge = Integer(tokens[2])
        topEdge = Integer(tokens[3])
        rightEdge = Integer(tokens[4])
        bottomEdge = Integer(tokens[5])
  
        newFrame = SpriteFrame.new(name, @frames.length, leftEdge, topEdge, rightEdge, bottomEdge)
        @frames << newFrame
        if name != UNTITLED_LINE
          @frameMap[name] = newFrame
        end
      elsif kind == "anim"
        animation_length = tokens.length() - 2
        animation_frames = Array.new(animation_length) {|i| @frameMap[tokens.last(animation_length)[i]]}
        @animations << SpriteAnimation.new(name, animation_frames)
      end
    end
  end

  def load_from_image(image)
    frameList = []
    pixelMappings = {}

    #iterate through image and create frames
    image.get_height().times do |y|
      image.get_width().times do |x|
        currPixel = PixelPair.new(x, y)

        if image.is_transparent(x,y)
          #if this pixel is transparent, it's not part of a region, so let's do the next one
          pixelMappings[currPixel] = nil
          next
        end

        pixelsAround = []

        threshold = 2

        (-threshold).upto(-1) do |yOff|
          (-threshold).upto(threshold) do |xOff|
            pixelsAround << PixelPair.new(x + xOff, y + yOff)
          end
        end

        (-threshold).upto(-1) do |xOff|
          pixelsAround << PixelPair.new(x + xOff, y)
        end

        northWestPixel = PixelPair.new(x - 1, y - 1)
        northPixel = PixelPair.new(x, y - 1)
        northEastPixel = PixelPair.new(x + 1, y - 1)
        westPixel = PixelPair.new(x - 1, y)
        
        regionFound = false
        pixelsAround.each do |pixel|
          next if pixelMappings[pixel] == nil

          if regionFound
            if pixelMappings[pixel] != pixelMappings[currPixel]
              #if the pixel above and the pixel to the left are distinct regions, the current pixel means they're the same region
              oldFrame = pixelMappings[pixel]
              mergedFrame = pixelMappings[currPixel]
              
              #merge the regions together, removing one altogether and relabeling all the other entries in that list
              frameList[mergedFrame].merge(frameList[oldFrame])
              frameList[oldFrame].pixels.each do |pixelToMerge|
                pixelMappings[pixelToMerge] = mergedFrame
              end

              #remove the old frame entry
              frameList[oldFrame] = nil
            end
          else
            #if this previous pixel is part of a region, the current pixel also is
            pixelMappings[currPixel] = pixelMappings[pixel]
            regionFound = true
          end
        end

        if regionFound == false
          #if the previous pixels aren't in regions, then this pixel is part of a new region we need to create
          pixelMappings[currPixel] = frameList.length
          frameList << SpriteFrame.new()
        end

        frameList[pixelMappings[currPixel]].insert(currPixel)
      end
    end

    @frames = frameList.compact
  end

  def get_num_frames()
    if @frames == nil
      return 0
    else
      return @frames.length
    end
  end

  def get_num_animations()
    return @animations.length
  end

  def get_frame(index)
    if @frames == nil
      return nil
    else
      if index.is_a?(Integer)
        return @frames.at(index)
      elsif index.is_a?(String)
        return @frameMap[index]
      end
    end
  end

  def get_animation(index)
    return @animations.at(index)
  end

  def add_animation()
    @animations << SpriteAnimation.new(UNTITLED_LINE, Array.new())
    return @animations.length - 1
  end

  def save_to_file(filename)
    file = File.new(filename, "w+")
    @frames.each do |frame|
      file.puts frame.to_s
    end
  end
end