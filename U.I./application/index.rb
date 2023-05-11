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
require 'application/tool_methods'

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
# current_matrix_id = grab(:current_matrix).data
# we initialize the matrix
current_matrix = build_matrix
# now we assign events to it
matrix_events(current_matrix)

# now we store the active matrix
grab(:active_matrix).data = 0

log "active matrix is #{grab(:active_matrix).data}"

# main methods

# tests
current_matrix.cell(1).text({ data: :hello, visual: { size: 12 } })
current_matrix.cell(9).text({ data: :cool, visual: { size: 12 } })
