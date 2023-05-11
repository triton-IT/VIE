# frozen_string_literal: true

def get_project(params)
  new_project_name = params[:name]
  grab(:current_project).data(new_project_name) # we update the project current project
  set_title
  Atome.controller_sender({ action: :load_project, id: id })
end

def new_project
  clear_all_matrix
  Atome.controller_sender({ action: :new_project })
end

def clear_all_matrix
  grab(:vie_matrix).cells.each do |cell_id|
    cell_found = grab(cell_id)
    cell_found.physical.each do |attached_atome_id|
      cell_found.delete({ id: attached_atome_id })
    end
  end
  # end
end

def clear_current_matrix
  alert :terminator
end

def clear_slot
  alert :clearing_the_slot
end

def new_matrix; end

def view_matrix(matrix_id)
  ;
end

def add_module(module_data)
  log "build module: #{module_data}"
  # we feed the project_matrix
  project_matrix = grab(:project_matrix)
  matrix_found = module_data[:position][:z]
  module_position = ((module_data[:position][:y]) * 8 + module_data[:position][:x])
  active_matrix = grab(:active_matrix).data
  # we test if the module should be placed on the current matrix if so we render it
  if active_matrix == matrix_found
    current_slot = grab("vie_matrix_#{module_position}")
    current_module = current_slot.box({ id: module_data[:id], attached: [:invisible_color], width: current_slot.width,
                                        height: current_slot.height, left: 0, top: 0 })
    module_properties = vie_styles[:module_label].merge({ data: module_data[:name] })
    current_module.text(module_properties)
    # we render the module
    # let's build a module
    icon_id = "icon_for_#{module_data[:id]}"
    current_module.box({ id: icon_id, width: current_slot.width / 1.7,
                         height: current_slot.height / 1.7, center: true })
    current_module.circle(width: 9, height: 8, center: true)
    # display_svg(get_icon(module_data[:type_id]), :lightgray, icon_id)
  end
  project_matrix.data[matrix_found] = { module_position => module_data } unless project_matrix.data[matrix_found]
  # alert project_matrix.data[matrix_found]
end

def add_link(link_data)
  log "build link_data: #{link_data}"

  # new_connection = grab(:connection).data
  # inspector.text(data: "linking : #{new_connection[0]} and #{new_connection[1]}", visual: { size: 12 })
  # detach_cell_color(current_matrix, source_cell, target_cell)
end

def clear_zone(zone)
  zone.materials.each do |child_found|
    grab(child_found)&.delete(true)
  end
  zone.delete(:materials)
end

def remove_active(items)
  items.each do |id_found, _properties|
    grab(id_found)&.detached(:active_color)
    grab(id_found)&.attached(:inactive_color)
  end
end

def link_cells(current_matrix, inspector, source_cell, target_cell)
  new_connection = grab(:connection).data
  inspector.text(data: "linking : #{new_connection[0]} and #{new_connection[1]}", visual: { size: 12 })
  detach_cell_color(current_matrix, source_cell, target_cell)
  # fake data below :
  add_link({ "id" => "GUID", "sourceComponentId" => 0, "sourceOutputSlotId" => 2,
             "targetComponentId" => 1, "targetInputSlotId" => 0, "active" => "true" })
end

def request_link_cell(current_matrix, inspector, index)
  grab(:connection).data[1] = index
  source_cell = grab(:connection).data[0]
  target_cell = grab(:connection).data[1]
  return if source_cell == target_cell

  clear_zone(inspector)
  link_cells(current_matrix, inspector, source_cell, target_cell)
end


