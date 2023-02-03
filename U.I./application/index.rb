# frozen_string_literal: true

require 'application/data_for_test'
require 'application/atome_experimental'
require 'application/version'
require 'application/styles'

default_project_name = :untiltled
# active element
# this the atome used to store the module to be associated
element(id: :connection, data: [1, 3])
# used to store current matrix ID the initial matrix id is : '0_0_0'
element(id: :current_matrix, data: ['0_0_0'])
# this the atome used to store the selected module
element(id: :selected, data: [])
# this the atome used to store the selected module
element(id: :context, data: :default)
# this the atome used to store the currentproject
element(id: :current_project, data: default_project_name)
# this the atome used to store the module linked
element(id: :links, data: {})

# Basic U.I. elements
box(vie_styles[:center_style])
box(vie_styles[:action_style])
box(vie_styles[:toolbox_style])
box(vie_styles[:inspector_style])
# box(vie_styles[:navigation_style])

# logo
logo_color = 'rgba(99, 255, 99, 0.3)'
grab(:action).box({ id: :logo_support, width: 33, height: 33, left: :auto, top: 7, right: 15, attached: :invisible_color })
svg_fetch(:vie, logo_color, :logo_support)

# project title
grab(:action).text(vie_styles[:title].merge({ data: default_project_name }))

# module_creation
# def resize_matrix(matrix, center)
#   if center.width.to_px > center.height.to_px
#     matrix.resize(center.height.to_px, center.height.to_px)
#   else
#     matrix.resize(center.width.to_px, center.width.to_px)
#   end
#   matrix.width(:auto)
#   matrix.right(0)
# end

def create_matrix(id)
  center = grab(:center)
  modules = center.matrix({ id: "vm_#{id}", top: 0, left: 0, smooth: 0, columns: { count: 8 }, rows: { count: 8 },
                            width: 440,
                            height: 440,
                            color: { alpha: 0 },
                            cells: { particles: { margin: 9, smooth: 3,
                                                  color: vie_styles[:cell_color],
                                                  shadow: vie_styles[:cell_shadow] } }
                          })

  # FIXME: when the matrix is attached to the parent it apply it's cell's color and shadow
  # patch below

  center.detached(:cell_color)
  center.detached(:cell_shadow)
  modules.width(:auto)
  modules.right(0)
  # $window.on :resize do
  #   resize_matrix(modules, center)
  # end
  # resize_matrix(modules, center)
  inspector = grab(:inspector)
  modules.children.value.each_with_index do |_module_found, index|
    modules.cell(index).touch(:down) do
      puts "down context is: #{grab(:context).data.value}"
      grab(:connection).data[0] = index
    end
    modules.cell(index).touch(:long) do
      puts "up context is: #{grab(:context).data.value}"
      if grab(:selected).data.value.include?(self.id.value)
        grab(:selected).data.value.each do |cell_id|

          selected_cell = grab(cell_id)
          selected_cell.attached([:cell_color])
          selected_cell.detached(:active_color)
        end
        grab(:selected).data([])
      else
        grab(:selected).data.value << self.id.value
        attached([:active_color])
        detached(:cell_color)
      end
    end
    modules.cell(index).touch(:up) do
      # puts "up context is: #{grab(:context)}.data.value"
      # puts "======>#{grab(:selected).data}"
      grab(:connection).data[1] = index
      source_cell = grab(:connection).data.value[0]
      dest_cell = grab(:connection).data.value[1]
      unless source_cell == dest_cell
        clear_zone(inspector)
        new_connection = grab(:connection).data.value
        vivify({ connect: "TODO :add matrix _id  #{new_connection}" })

        grab(:links).data[modules.cell(index).id.value] = [source_cell,dest_cell]
        grab(:connection).data << [source_cell,dest_cell]

        inspector.text(data: new_connection)
        modules.cell(source_cell).detached(:cell_color)
        modules.cell(dest_cell).detached(:cell_color)
        modules.cell(source_cell).attached(:cell_connected)
        modules.cell(dest_cell).attached(:cell_connected)
        wait 0.6 do
          modules.cell(source_cell).attached(:cell_color)
          modules.cell(dest_cell).attached(:cell_color)
          modules.cell(source_cell).detached(:cell_connected)
          modules.cell(dest_cell).detached(:cell_connected)
        end

      end
    end
  end

end

# id is the module position in prev module first int is the parent matrix id , second int is the row in prev matrix
# third is the column position
create_matrix('0_0_0')

# now the fill the U.I.

def set_title
  project_name = grab(:current_project).data
  grab(:title).data(project_name)
end

# utils for vie

# sever operations

def vivify(params)
  # this method serialize data to jason and send it to back
  puts "send to back ==> #{params}"
end

def load_project(data)
  project_id = data[:id]
  vivify(project_id)
  new_project_name = data[:name]
  grab(:current_project).data(new_project_name) # we update the project current project
  set_title
end

def fill_toolzone (ids)
  # the ids are used to retrieve the icon and look the the corresponding method when touch
  svg_color = 'rgba(180, 120, 50, 1)'
  margin = vie_styles[:margin]
  support_style = vie_styles[:support_style]
  icon_spacing = vie_styles[:support_style][:height] + margin * 2
  ids.each_with_index do |id_found, index|
    support = grab(:toolbox).box(support_style.merge({ top: (icon_spacing * index) + margin, id: "tool_support_#{index}" }))
    svg_fetch(id_found, svg_color, support.id)
    support.touch(true) do
      send("action_#{id_found}")
    end
  end
end

fill_toolzone([:folder, :clear, :settings, :edition, :select, :group,:link, :copy, :paste, :undo])

def action_clear
  grab(:selected).data.each do |selected_slot|
    grab(selected_slot).children.each do |child_found|
      grab(child_found).delete(true) if grab(child_found)
    end
  end
end

def action_link
  alert grab(:links).data
end

def clear_zone(zone)
  zone.children.each do |child_found|
    grab(child_found).delete(true) if grab(child_found)
  end
  zone.children = []
end

def remove_active(items)
  items.each do |id_found, _properties|
    grab(id_found).detached(:active_color) if grab(id_found)
    grab(id_found).attached(:inactive_color) if grab(id_found)
  end
end

def list(parent, style, items)
  margin = vie_styles[:margin]
  items.each_with_index do |(id_found, properties), index|
    label = properties[:name]
    item = parent.text({ data: label, id: id_found }.merge(style))
    item.top(25 * index + margin)
    item.touch(true) do
      remove_active(items)
      # we set the current project
      detached('inactive_color')
      item.attached([:active_color])
      send(properties[:action], properties.merge({ id: id_found }))
    end
  end
end

# main methods
def action_folder
  inspector = grab(:inspector)
  clear_zone(inspector)
  text_list_style = vie_styles[:list_style].merge({ classes: :project_list })
  list(inspector, text_list_style, project_list)
end

def rename_project
  alert "rename : #{grab(:current_project).data}"
end

def delete_project
  alert "delete : #{grab(:current_project).data}"

end

def duplicate_project
  alert "duplicate : #{grab(:current_project).data}"

end

def action_settings
  inspector = grab(:inspector)
  clear_zone(inspector)
  text_list_style = vie_styles[:list_style].merge({ classes: :settings_list })
  project_name = grab(:current_project).data.value
  # project_name=project_list[grab(:current_project).data.value][:name] if project_list[grab(:current_project).data.value]
  list(inspector, text_list_style, {
    rename: { name: "rename : #{project_name}", data: :item1, action: :rename_project },
    delete: { name: "delete", data: :item2, action: :delete_project },
    duplicate: { name: "duplicate", data: :item3, action: :duplicate_project },
  })
end

def action_edition
  inspector = grab(:inspector)
  clear_zone(inspector)
  svg_color = 'rgba(180, 120, 50, 1)'
  margin = vie_styles[:margin]
  support_style = vie_styles[:support_style]
  icon_spacing = vie_styles[:support_style][:height] + margin * 2
  index = 0
  tool_list.each_value do |data|
    icon_found = data[:icon]
    action_found = data[:action]
    id_found = data[:id]
    support = grab(:inspector).box(support_style.merge({ top: (icon_spacing * index) + margin, id: "module_support_#{index}" }))
    svg_fetch(icon_found, svg_color, support.id)
    support.touch(true) do
      send(action_found, id_found)
    end
    # support.depth(5)
    support.drag(true)
    index += 1
  end
end

def insert_module(module_id)
  grab(:selected).data.each do |module_id_found|
    module_found = grab(module_id_found)
    module_found.children.each do |child_found|
      grab(child_found).delete(true) if grab(child_found)
    end
    tool_found = tool_list[module_id][:icon]
    tool_color= :orange
    module_found.box({id: "#{module_found.id.value}_svg_support", width: module_found.width.value/2, height: module_found.height.value/2, center: true, attached: :invisible_color})
    svg_fetch(tool_found, tool_color, "#{module_found.id.value}_svg_support")
  end

end

def action_select
  inspector = grab(:inspector)
  clear_zone(inspector)
end

def action_group
  inspector = grab(:inspector)
  clear_zone(inspector)
end

def action_copy
  inspector = grab(:inspector)
  clear_zone(inspector)
end

def action_paste
  inspector = grab(:inspector)
  clear_zone(inspector)
end

def action_undo
  inspector = grab(:inspector)
  clear_zone(inspector)
end

# alert filer
# grab(:tool_zone).image({ path: "./medias/images/icons/folder.svg", top: 15, left: 0, size: 24 })

# files.touch(true) do
#   grab(:filer).color(:red)
# end
# module
# filer.image({ path: "./medias/images/icons/modules.svg", top: 63, left: 15, size: 24 })

# edit
# filer.image({ path: "./medias/images/icons/edit.svg", top: :auto, bottom: 15, left: 23, size: 24 })
# filer.text({ data: "Mon projetlkjlkjl", top: :auto, bottom: 15, left: 23, size: 24 })
# file_list=inspector
# file_list.text({visual: {size:  15 }, data: "mon projet", left: 9, top: 6, attached: [txt_color.id]})
# file_list.text({visual: {size:  15 }, data: "mon projet", left: 9, top: 36, attached: [txt_color.id]})
# file_list.text({visual: {size:  15 }, data: "mon projet", left: 9, top: 66, attached: [txt_color.id]})
# module_list=box.box(list_style)
# tool_list=box.box(list_style)
# listing = inspector.box(list_style)
#
# def list(parent, style, items)
#   items.each_with_index do |(k, v), index|
#     item = parent.text(style)
#     item.top(25 * index)
#     item.touch(true) do
#       item.color(:lightgray)
#     end
#   end
# end
#
# text_list_style = { visual: { size: 15 }, data: "mon projet", left: 9, top: 6, attached: [txt_color.id] }
# list(listing, text_list_style, {
#   i1: { title: "mon projet", data: :item1, action: :my_action },
#   i2: { title: "mon projet2", data: :item1, action: :my_action },
#   i3: { title: "autre projet", data: :item1, action: :my_action },
#   i4: { title: "my great song", data: :item1, action: :my_action },
# })

# FIXME : impossible to create more than one matrix on a single page
# FIXME : when adding a matrix the parent get its color
# FIXME : when adding a shadow to matrix cells the parent of the parent get the shadow too

# TODO : text should be define in 'vw' to resize according to it's parent
# TODO : Children can't be removed , idem for attach
# TODO : remmove doesn't work with shadow

# FIXME:






