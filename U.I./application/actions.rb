# frozen_string_literal: true

def action_router(id_found, options)
  result_zone = grab(options[:result_zone])
  clear_zone(result_zone) if result_zone
  # this is the main action methods that can debug sanitize and route to the corresponding method
  send("action_#{id_found}", result_zone)
  # we set the context
  grab(:context).data[:mode] = id_found
  # if debug mode then we log the above messages
  log("action_router say, call :  action_#{id_found}\nNew  context: #{grab(:context).data}")
end

# tools actions
def action_load(_targeted_zone)
  # this method get user project list
  Atome.controller_sender({ action: :get_projects })
end

def action_clear(_targeted_zone)
  # this delete the currently selected slots
  grab(:selected).data.each do |selected_slot|
    grab(selected_slot).materials.each do |child_found|
      grab(child_found)&.delete(true)
    end
  end
end

def action_settings(_targeted_zone)
  Atome.controller_sender({ action: :get_settings })
end

def action_edition(_targeted_zone)
  Atome.controller_sender({ action: :get_modules })
end

def action_select(_targeted_zone) end

def action_group(_targeted_zone) end

def action_copy(_targeted_zone) end

def action_paste(_targeted_zone) end

def action_undo(_targeted_zone) end

# sub actions
def action_get_modules(_targeted_zone)
  Atome.controller_sender({ action: :get_modules })
end
