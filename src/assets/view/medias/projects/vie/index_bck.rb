view = grab(:view)
style = { back_color: { red: 0.2, green: 0.2, blue: 0.2 }, shade: { x: 0, y: 0, blur: 9, color: "rgba(0,0,0,0.6)" }, tool_size: 39 , noise: { opacity: 0.15, intensity: 39 , color: :black} }
style=Intuition.style
offset = 6
workspace = Matrix.new({ x: 0, y: 0, nb: 8, size: 1200, background: nil, name: :matrix_2, margin: 6, style: { color: { red: 1, green: 1, blue: 0, alpha: 0.3 }, smooth: 3, border: { color: :black, thickness: 0, pattern: :solid } } })
# view = box({ width: "100%", height: "100%", opacity: 0,color: style[:back_color], atome_id: :view_background, z: -6 })
view.color(style[:back_color])
view.noise(style[:noise])


# alert Intuition.style
# wait 0.6 do
#   grab(:view).animate({
#                         start: { opacity: 0 },
#                         end: { opacity: 1 },
#                         duration: time,
#                         loop: 0,
#                         curve: :backOut,
#                         target: :view_background
#                       })
# end

workspace.matrix.xx(offset)
workspace.matrix.y(offset)
workspace.matrix.shadow(style[:shade])

workspace.cells.color(style[:back_color])
workspace.cells.noise(style[:noise])
workspace.cells.data(false)
# my_cell.data({empty: false})
#test
my_cell = workspace.get_cell(3)
my_cell.color(:orange)
my_cell.noise(style[:noise])
my_cell.data(sample: :ok)

my_cell2 = workspace.get_cell(1)
my_cell2.color(:red)
my_cell2.noise(style[:noise])
my_cell2.data(sample: :kool)

my_cell3 = workspace.get_cell(5)
my_cell3.color(:orange)

my_cell7 = workspace.get_cell(7)
my_cell7.data(sample: :menu)
my_cell7.color(:purple)
# img = my_cell3.image(:boat)
# wait 0.2 do
# img.size(:fit)
# end

my_cell4 = workspace.get_cell(6)
my_cell4.data(true)
my_cell4.color(:yellow)




my_cell3.data(sample: :good)
#test

workspace.cells.each do |current_cell|
  current_cell.touch do
    cell_data = self.data
    # grab(atome_id).text({ content: cell_data.to_s, visual: 12 })
    # self.text(data)
    case cell_data
    when false
      self.color(:blue)
      self.noise(style[:noise])
      self.data(true)
    when true
      self.color(:green)
      self.noise(style[:noise])
      self.data(false)
    when -> (h) { h[:sample] == :ok }
      open_list
    when -> (h) { h[:sample] == :kool }
      s = grab(:intuition).slider({
                                    atome_id: :tututu,
                                    function: :rotate,
                                    target: :matrix_2_1,
                                    # target: [:the_box, :the_circle],
                                    # target: :selection,
                                    type: :circular,
                                    length: 39,
                                    thickness: 12,
                                    helper_length: nil,
                                    helper_thickness: 12,
                                    value_size: 15,
                                    color: "rgb(99, 99, 99, 0)",
                                    back_color: "rgba(66,66,66,0.6)",
                                    range_color: "orange",
                                    helper_color: "orange",
                                    value_color: :orange,
                                    value: 90,
                                    unit: "px",
                                    min: 0,
                                    max: 360,
                                    smoothing: 3
                                  })
      s.x(150).y(33)

    when -> (h) { h[:sample] == :menu }

    toto = circleliser [:home_tool, :tools_tool, :communication_tool, :help_tool, :capture_tool, :view_tool, :time_tool, :search_tool], 222, 33, 1,:impulse_tool
      toto.drag(true)
    else
      alert :empty
    end
  end
end

# inspector = box(atome_id: :inspector)
# inspector.shadow(style[:shade])
# inspector.color(style[:back_color])

# toolbox = box(atome_id: :toolbox)
# toolbox.shadow(style[:shade])
# toolbox.color(style[:back_color])
# toolbox.height = view.convert(:height)
# toolbox.width(style[:tool_size])

def set_ui_size(view, workspace, offset)
  view_width = view.convert(:width)
  view_height = view.convert(:height)
  if view_width > view_height
    workspace.resize_matrix(view_height - offset)
  else
    workspace.resize_matrix(view_width - offset)
  end
  # inspector.height(view_height)
  # inspector.width(view_width - (workspace.matrix.width + offset * 2))
  # toolbox.height = view_height
end

#
set_ui_size(view, workspace, offset)
ATOME.resize_html do |evt|
  set_ui_size(view, workspace, offset)
end

############### the box ######
# b = box({ atome_id: :the_box, x: 60, y: 99, color: :green })
#
# b.touch do
#   b.color(:red)
# end
#
# b.parameter({ target: :the_box,
#               type: :circular,
#               length: 33,
#               thickness: 12,
#               helper_length: nil,
#               helper_thickness: 12
#             })
#
# t = text(" ")
#
# b.touch do
#   t.content(parameter.to_s)
# end

# atome method generator

# generate(:my_meth ) do |val|
#   self.text ({ content: val, x: 12, y: 12} )
# end

# b.my_meth(:kool)

# class Atome
#
#   def poilu val
#     puts :kool
#   end
# end




# class Atome
#
#   def list(value = nil, password = nil, &proc)
#   end
# end
#alternate list as rails?

# while i < 9
#   i += 1
#   text ({ content: "kool #{i}", atome_id: "the_text_#{i}" })
# end
def open_list

  # line {height: 33, x: 3, margin: 3}
  listing = grab(:intuition).list({ atome_id: :the_listing,title: "dummy list",
                                    line: { height: 30 }, color: :orange, x: 333, y: 99, width: 240, height: 123,
                                    content: [{ value: :the_text_1, :action => lambda do
                                      alert :ok_so_good
                                    end }, { value: :the_text_2, :action => lambda do
                                      alert :nice
                                    end }, { value: :the_text_3, :action => lambda do
                                      alert :nice
                                    end }, { value: :the_text_4, :action => lambda do
                                      alert :ok
                                    end }, { value: :the_text_5, :action => lambda do
                                      alert :nice
                                    end }, { value: :the_text_6, :action => lambda do
                                      alert :now
                                    end }, { value: :the_text_7, :action => lambda do
                                      alert :why
                                    end }, { value: :the_text_8, :action => lambda do
                                      alert :good
                                    end }, { value: :the_text_9, :action => lambda do
                                      alert :also
                                    end }, { value: :the_text_10, :action => lambda do
                                      alert :well
                                    end }
                                    ]
                                  })
  listing.x(60)
  listing.y(300)
end

# ############# view mode ####################

# c = circle({ x: 333, y: 333, atome_id: :the_circle })
# c.text(:hello)
#
# c.render({
#            mode: :list,
#            list: :property,
#            sort: :alphabetically
#          })

#

# ################ Key test
# t = text("hello")
# t.x = 233
# t.y = 33
# t.edit(true)
# t.key do |e|
#   if e.key_code == 13
#     return false
#   end
#   # t.content(:ok)
#   # e.stop
# end

# ################## select test ##########
# select = grab(:intuition).circle({ x: 150, y: 240 })
# treated_item = []
# select.touch do
#   grab(:view).selectable(true) do
#     selection.each do |atome_found|
#       treated_item << atome_found
#     end
#     selection.color([:yellowgreen, :green]).drag(true).shadow({ color: :black, thickness: 0, x: 0, y: 0, blur: 6 })
#   end
# end
#
#
# ######## stop verif
# # stop.touch(option: :up) do
# #   stop.content("selection desactivated").color(:red)
# #   start.content("click me to start selecting").color(:black)
# #   treated_item.each do |item_found|
# #     item_found.color(:red).drag(:destroy)
# #   end
# #   grab(:view).selectable(:destroy)
# # end

# ############# Matrix test ####################
#
# # matrix example
# # example below
# box({y: 333, atome_id: :the_box, drag: true})
# view = grab(:view)
# exemple_1 = Matrix.new({ parent: :the_box, nb: 4, size: 333,cell_width: 99, cell_height: 33, background: nil, name: :the_matrix, margin: 33, style: { color: { red: 1, green: 1, blue: 0, alpha: 1 }, smooth: 9,border: { color: :black, thickness: 3, pattern: :solid }} })
# exemple_1.matrix.y(66)
# exemple_1.matrix.drag(true)
# matrix = Matrix.new({ nb: 4, size: 333,background: nil, name: :matrix_2, margin: 33, style: { color: { red: 1, green: 1, blue: 0, alpha: 0.3 }, smooth: 3,border: { color: :black, thickness: 0, pattern: :solid }} })
# exemple_1.matrix.color(:red)
# matrix.matrix.center(true)
# matrix.cells.color(:violet)
# my_cell = matrix.get_cell(3)
# my_cell.color(:blue)
# ATOME.resize_html do |evt|
#   matrix.resize_matrix(view.convert(:width)/2)
#   matrix.matrix.center(true)
# end
#
#
# def view_pix object
#   object.child.each do |cell|
#     cell.child.each do |back|
#       back.touch do
#         object.blur(3)
#         object.opacity(0.5)
#         big_pix = image({ content: back.content.sub("_", "_big_"), size: "100%" })
#         big_pix.touch do
#           object.blur(0)
#           object.opacity(1)
#           big_pix.delete(true)
#         end
#       end
#     end
#   end
# end

##################### circleliser ###########################

def close_menu(centered_object, list, time)
  list.each do |data|
    item_id = data[:atome_id]
    item = grab(data[:atome_id])
    found_pos_x = data[:end_x] - centered_object.width / 2 + item.width / 2
    found_pos_y = data[:end_y] - centered_object.height / 2 + item.height / 2
    # found_pos_x=data[:end_x]
    # found_pos_y=data[:end_y]
    # alert "#{item.x} : #{found_pos_x}"
    # item.x=0
    # item.y=0
    grab(:view).animate({
                          start: { x: found_pos_x, y: found_pos_y },
                          end: { x: 0, y: 0},
                          duration: 1000,
                          loop: 0,
                          # curve: :backOut,
                          target: item_id
                        })
    wait time / 1000 do
      centered_object.delete(true)
    end

  end

  # wait 2 do
  #   time= 600
  #   grab(:view).animate({
  #                         start: { x: item.x, y: item.y, opacity: 1 },
  #                         end: { x: 0, y: 0, opacity: 0 },
  #                         duration: time,
  #                         loop: 0,
  #                         curve: :backOut,
  #                         target: item_id
  #                       })
  #   wait time/1000 do
  #     circleliser_support.delete(true)
  #   end
  # end
end

def circleliser objects_list, size, obj_size,offset, center_obj = nil
  nb_obj = objects_list.length
  item_style = { color: { red: 0.3, green: 0.3, blue: 0.3, alpha: 0.15 }, smooth: 9, shadow: { bounding: true, add: :true, x: 0, y: 0, color: "rgba(0,0,0,0.6)", blur: 9 } }
  # we create the support for the contained items
  circleliser_support = grab(:intuition).circle({ size: size, color: :transparent, x: 120, y: 120 })

  radius = size / 2
  alpha = 2 * (Math::PI / nb_obj)
  item_list = []
  time = 600

  # We create an object in the center of the circle if needed support for the contained items
  if center_obj
    centered_item = grab(center_obj)
    circleliser_support.attach(center_obj)
    # centered_item = circleliser_support.box(item_style)
    # centered_item.size(39)
    # impulse_icon = centered_item.image(:atome)
    # impulse_icon.size(24)
    # impulse_icon.center(true)
    # impulse_icon.color(:orange)

    centered_item.set({ smooth: item_style[:smooth], color: item_style[:color], x: radius - centered_item.width / 2, y: radius - centered_item.height / 2 })
    centered_item.touch do
      close_menu(circleliser_support, item_list, time)
    end
  end
  index = 0
  # alert objects_list.length
  # while index < nb_obj
  objects_list.each_with_index do |item_id, index|


    # index += 1
    item = grab(item_id)
    # alert "problem below"
    circleliser_support.attach(item_id)
    # circleliser_support.attach(:hello)

    item.set({ smooth: item_style[:smooth] })
    start_x = radius - item.width / 2
    start_y = radius - item.height / 2
    computed_offset = Math::PI / (nb_obj / offset)
    end_x = (radius + Math.cos(alpha * index + computed_offset) * radius) - item.width / 2
    end_y = (radius + Math.sin(alpha * index + computed_offset) * radius) - item.height / 2
    item.x(start_x)
    item.y(start_y)


    # wait 3 do
    #   item.x(end_x)
    #   item.y(end_y)
    # end
    item_list << { atome_id: item_id, start_x: start_x, start_y: start_y, end_x: end_x, end_y: end_y }
    #
    # x_open_position = item[:end_x]
    # y_open_position = item[:end_y]
    # grab(tool_item[:atome_id]).x = central_position_x
    # grab(tool_item[:atome_id]).y = central_position_y
    grab(:view).animate({
                          start: { x: 0, y: 0 },
                          end: { x: end_x - (size/2 - item.width/ 2) , y: end_y - (size/2 -item.height/2) },
                          duration: time,
                          loop: 0,
                          curve: :backOut,
                          target: item_id
                        })
  end
  # anim
  # objects_list.each do |tool_item_id|
  # alert central_position
  # tool_item=grab(tool_item_id)
  # x_open_position = tool_item[:end_x]
  # y_open_position = tool_item[:end_y]
  # # grab(tool_item[:atome_id]).x = central_position_x
  # # grab(tool_item[:atome_id]).y = central_position_y
  # grab(:view).animate({
  #                start: { x: 0, y: 0 },
  #                end: { x: x_open_position-size/2.7, y: y_open_position-size/2.7},
  #                duration: 300,
  #                loop: 0,
  #                curve: :backOut,
  #                target: tool_item_id
  #              })
  # wait 2 do
  #   item.animate({
  #                  end: { x: central_position, y: central_position },
  #                  start: { x: x_open_position, y: y_open_position },
  #                  duration: 3000,
  #                  loop: 0,
  #                  curve: :backOut,
  #                  target: tool_item[:atome_id]
  #                })
  # end
  # end
  return circleliser_support
end

wait 0.3 do

  grab(:intuition).tool({ name: :impulse, icon: :atome, x: 30, yy: -330, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    # alert self.atome_id
  end })
  tool_1=grab(:intuition).tool({ name: :home, icon: :home, x: 30, yy: -330, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    alert self.atome_id
  end })
  tool_1.noise(Intuition.style[:noise])
  tool_2=grab(:intuition).tool({ name: :tools, icon: :tools, x: 30, yy: -330, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    alert self.atome_id
  end })
  tool_2.noise(Intuition.style[:noise])
  tool_3=grab(:intuition).tool({ name: :communication, icon: :communication, x: 30, yy: -330, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    alert self.atome_id
  end })
  tool_3.noise(Intuition.style[:noise])
  tool_4=grab(:intuition).tool({ name: :help, icon: :help, x: 30, yy: -330, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    alert self.atome_id
  end })
  tool_4.noise(Intuition.style[:noise])
  tool_5=grab(:intuition).tool({ name: :capture, icon: :capture, x: -330, yy: 30, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    alert self.atome_id
  end })
  tool_5.noise(Intuition.style[:noise])
  tool_6=grab(:intuition).tool({ name: :view, icon: :view, x: 30, yy: -330, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    alert self.atome_id
  end })
  tool_6.noise(Intuition.style[:noise])
  tool_7=grab(:intuition).tool({ name: :time, icon: :time, x: 30, yy: -330, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    alert self.atome_id
  end })
  tool_7.noise(Intuition.style[:noise])
  tool_8=grab(:intuition).tool({ name: :search, icon: :search, x: 30, yy: -330, z: 3, blur: Intuition.style[:blur], :activate => lambda do
    alert self.atome_id
  end })
  tool_8.noise(Intuition.style[:noise])
  # grab(:impulse_tool).activation(true)
  # activation(grab(:impulse_tool))

end


# b=box()
t=text({content: :hello,atome_id: :hello, x: 99, y: 99})
t.visual(99)
# t=box
# t.color(:red)

# t.color(:red)
t.shadow(Intuition.style[:shade])
t.noise({ opacity: 1, intensity: 39 , color: :black})
# alert t.color
# t.width(33)
# b.attach(:hello)
# # noise test
#
#
# b=box
# b.overflow(:hidden)
#
# i=grab(:view).image(:noise)
# i.opacity(0.3)
# i.width(3333)
# i.height(3333)
##### parent test
# b=box({width: "100%", height: "100%", color: :red })
# b.noise( { opacity: 1, intensity: 39 , color: :black})
# c=circle({width: "100%", height: "100%" })
# c.parent(b.atome_id)
history= Quark.get_history
time_get =history[0][:atome][:time]
formated_time=Time.at(time_get)
text ({ content: formated_time, x: 0, color: :orange })
text ({ content: history[33].to_s, y: 33, color: :orange  })

b=box
b.x=b.y=99
b.drag(true) do |evt|
  puts b.y
  if b.y < 66
    b.y=0
  else
    b.y=b.y
  end
end




########## table test


item = <<-DATA.chomp
Ampli Point to point
Matériaux nobles
Simulation HP
Variation des # avec pedale expression
Automatisation EQ 6 band /EQ 2 band
Automatisation Gain / Pas de gain
Automatisation Boost / pas de boost
2 canaux / 1 canal
Automatisation Master Volume / pas de master
Atténuateur / pas d’atténuateur
Automatisation Presence / Pas de presence]
itemq
Ampli
DATA
verif = item.split("\n")

  e = Atome.new
  # tab=e.table({columns: [{ "1"=> "fonctionalité" }, "prix de base", "prix option", "envie"]})
  tab = e.table({atome_id: :the_table, columns: [["kool", "super"], verif, [:so_good], [:hello, "ok "]] })
  tab.column({ add: 3, at: 1, content: ["super", "great", "add me"] })
  tab.row({ add: 3, at: 2, content: [:cool, "dac"] })
  tab.x = tab.y = 99



