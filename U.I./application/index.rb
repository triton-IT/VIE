# frozen_string_literal: true

require 'application/fake_data'

require 'application/experimental_api'
require 'application/version'
require 'application/styles'
require 'application/ui_template'
require 'application/utilities'
require 'application/communication'
require 'application/callbacks'
require 'application/events'
require 'application/actions'

# set demo mode:
require 'application/demo_methods'

# set debug mode:
grab(:debug).data[:debug] = true

# vie :

# Set the background color
grab(:view).attached(:back_color)

default_project_name = :untitled
# active element

# project title
grab(:action).text(vie_styles[:title].merge({ data: default_project_name }))

# tools

tools_list = {
  load: { result_zone: :inspector },
  clear: { result_zone: nil },
  settings: { result_zone: :inspector },
  edition: { result_zone: :inspector },
  select: { result_zone: nil },
  group: { result_zone: nil },
  copy: { result_zone: nil },
  paste: { result_zone: nil },
  undo: { result_zone: nil }

}

fill_tool_zone(tools_list)

# we ask the atome named ':current_matrix' for the current active matrix ID
current_matrix_id = grab(:current_matrix).data
# we initialize the matrix
current_matrix = create_matrix(current_matrix_id)
# now we assign events to it
matrix_events(current_matrix)

# utils for vie
#
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

# main methods

# tests
