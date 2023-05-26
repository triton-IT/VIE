# frozen_string_literal: true

def action_touch_router(id_found, options)
  current_tool = grab(id_found)
  # alert current_tool
  # TODO :  finalize SVG type to be able to colorize it
  tools=grab(:tools).data
  # alert "tool list is : #{tools}"
  tools.each_key do |tool|
    grab(tool).detached(:active_color)
    grab(tool).attached(:inactive_color)
  end
  current_tool.detached(:inactive_color)
  current_tool.attached(:active_color)
  # alert current_tool


  result_zone = grab(options[:result_zone])
  clear_zone(result_zone) if result_zone
  # this is the main action methods that can debug sanitize and route to the corresponding method
  send("action_touch_#{id_found}", result_zone)
  # we set the context
  grab(:context).data[:mode] = id_found
  # if debug mode then we log the above messages
  log("action_router say, call :  action_#{id_found}\nNew  context: #{grab(:context).data}")
end

def action_long_touch_router(id_found, _options)
  send("action_long_#{id_found}")
end

# tools touch actions
def action_touch_load(_targeted_zone)
  # this method get user project list
  Atome.controller_sender({ action: :get_projects })
end

def action_touch_clear(_targeted_zone)
  # this delete the currently selected slots
  grab(:selected).data.each do |selected_slot|
    grab(selected_slot).materials.each do |child_found|
      grab(child_found)&.delete(true)
    end
  end
end

def action_touch_settings(_targeted_zone)
  Atome.controller_sender({ action: :get_settings })
end

def action_touch_edition(_targeted_zone)
  Atome.controller_sender({ action: :get_modules })
end

def action_touch_select(_targeted_zone) end

def action_touch_group(_targeted_zone) end

def action_touch_copy(_targeted_zone) end

def action_touch_paste(_targeted_zone) end

def action_touch_undo(_targeted_zone) end

# tools long touch actions
def action_long_load(_targeted_zone) end

def action_long_clear(_targeted_zone) end

def action_long_settings(_targeted_zone) end

def action_long_edition(_targeted_zone) end

def action_long_select(_targeted_zone)
  selection = grab(:selected).data
  current_matrix = grab(:vie_matrix)
  select_all=true
  if selection.length == 64
    select_all=false
  end
  current_matrix.cells.each do |module_slot_id|
    slot_found = grab(module_slot_id)
    if select_all
      select_cell(slot_found, selection)
    else
      deselect_cell(slot_found, selection)
    end

  end

end

def action_long_group(_targeted_zone) end

def action_long_copy(_targeted_zone) end

def action_long_paste(_targeted_zone) end

def action_long_undo(_targeted_zone) end

# sub actions
def action_touch_get_modules(_targeted_zone)
  Atome.controller_sender({ action: :get_modules })
end
