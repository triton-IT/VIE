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

tools=grab(:tools).data
fill_tool_zone(tools)

# we ask the atome named ':current_matrix' for the current active matrix ID
# current_matrix_id = grab(:current_matrix).data
# we initialize the matrix
current_matrix = build_matrix
# now we assign events to it
matrix_events(current_matrix)

# now we store the active matrix
grab(:active_matrix).data = 0

log "active matrix is #{grab(:active_matrix).data}"
# logo
logo_color ={ red: 0.38, green: 1, blue: 0}
logo = <<~STR
  <path id="vieCanvas-bezier" stroke="rgb(0, 0, 0)" stroke-width="1" stroke-miterlimit="10" fill="rgb(255, 0, 0)" d="M 73.04,26.41 C 50.57,12.14 15.77,53.39 15.81,85.33 15.83,107.68 23.49,124.45 35.37,139.38 97.06,203.55 73.1,232.52 109.61,231.71 134.09,231.16 181.15,134.57 220.5,138.31 232.63,123.3 240.52,106.7 240.5,85.07 240.5,84.51 240.49,83.95 240.47,83.4 211.52,29.92 146.74,182.8 114.45,179.38 69.64,174.65 90.68,37.61 73.04,26.41 Z M 172.32,76.13 C 172.32,94.63 157.34,109.64 138.85,109.64 120.36,109.64 105.37,94.63 105.37,76.13 105.37,57.62 120.36,42.62 138.85,42.62 157.34,42.62 172.32,57.62 172.32,76.13 Z M 172.32,76.13" />
STR

logo2=grab(:action).vector({ width: 133, height: 133, left: :auto, top: 7, right: 15,id: :logo, definition: logo, color: logo_color})

# main methods

# tests
# current_matrix.cell(1).text({ data: :hello, visual: { size: 12 } })
# current_matrix.cell(9).text({ data: :cool, visual: { size: 12 } })
#

