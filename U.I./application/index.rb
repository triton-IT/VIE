# frozen_string_literal: true

require 'application/fake_data.rb'
require 'application/experimental_api'
require 'application/version'
require 'application/styles'
require 'application/ui_template'
require 'application/communication'
require 'application/callbacks'
require 'application/matrix_events'
require 'application/actions'

#vie :
#
# Set the background color
grab(:view).attached(:back_color)

default_project_name = :untitled
# active element

# project title
grab(:action).text(vie_styles[:title].merge({ data: default_project_name }))

# tools
fill_tool_zone(%i[load clear settings edition select group copy paste undo])


# we ask the atome named ':current_matrix' for the current active matrix ID
current_matrix_id=grab(:current_matrix).data
# we initialize the matrix
current_matrix=create_matrix(current_matrix_id)
# now we assign events to it
matrix_events(current_matrix)

# utils for vie
#
def clear_zone(zone)
  zone.materials.each do |child_found|
    grab(child_found).delete(true) if grab(child_found)
  end
  zone.delete(:materials)
end

def remove_active(items)
  items.each do |id_found, _properties|
    grab(id_found).detached(:active_color) if grab(id_found)
    grab(id_found).attached(:inactive_color) if grab(id_found)
  end
end

# main methods

# tests



