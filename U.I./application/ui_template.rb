# frozen_string_literal: true

# logo
logo_color = 'rgba(99, 255, 99, 0.3)'

grab(:action).box({ id: :logo_support, width: 33, height: 33, left: :auto, top: 7, right: 15,
                    attached: :invisible_color })

logo = <<~STR
  <svg  xmlns="http://www.w3.org/2000/svg" viewBox="0 0 256 256" >
  <path id="vieCanvas-bezier" stroke="rgb(0, 0, 0)" stroke-width="1" stroke-miterlimit="10" fill="rgb(255, 0, 0)" d="M 73.04,26.41 C 50.57,12.14 15.77,53.39 15.81,85.33 15.83,107.68 23.49,124.45 35.37,139.38 97.06,203.55 73.1,232.52 109.61,231.71 134.09,231.16 181.15,134.57 220.5,138.31 232.63,123.3 240.52,106.7 240.5,85.07 240.5,84.51 240.49,83.95 240.47,83.4 211.52,29.92 146.74,182.8 114.45,179.38 69.64,174.65 90.68,37.61 73.04,26.41 Z M 172.32,76.13 C 172.32,94.63 157.34,109.64 138.85,109.64 120.36,109.64 105.37,94.63 105.37,76.13 105.37,57.62 120.36,42.62 138.85,42.62 157.34,42.62 172.32,57.62 172.32,76.13 Z M 172.32,76.13" />
  </svg>
STR
display_svg(logo, logo_color, :logo_support)

# Basic U.I. elements
box(vie_styles[:center_style]) # the playground,( where the matrix lies)
box(vie_styles[:action_style]) # the action zone (the top bar)
box(vie_styles[:toolbox_style]) # the toolbox zone at the left side of the screen
box(vie_styles[:inspector_style]) # the inspector where at the right of the tool zone

# this the atome used to store the module to be associated
element(id: :connection, data: [1, 3])
# used to store current matrix ID (initial matrix id is : '0_0_0')
element(id: :current_matrix, data: ['0_0_0'])
# this the atome used to store the selected module
element(id: :selected, data: [])
# this the atome used to store the selected module
element(id: :context, data: { mode: :default, event: :released })
# this the atome used to store the current project
element(id: :current_project)
# # this the atome used to store the module linked
element(id: :debug, data: { debug: false })

def add_icons_to_tool_zone(tool, index, tool_style)
  tool_name = tool[0]
  svg_color = tool_style[:color]
  support_style = tool_style[:style]
  icon_spacing = tool_style[:spacing]
  margin = tool_style[:margin]
  support = grab(:toolbox).box(support_style.merge({ top: (icon_spacing * index) + margin,
                                                     id: "tool_support_#{tool_name}" }))
  # TODO: important for future use  keep to code below to fetch items
  display_svg(get_icon(tool_name), svg_color, support.id)
  options = tool[1]
  tool_events(support, tool_name, options)
end

def fill_tool_zone(tools_list)
  # the ids are used to retrieve the icon and look the the corresponding method when touch
  svg_color = 'rgba(180, 120, 50, 1)'
  margin = vie_styles[:margin]
  support_style = vie_styles[:support_style]
  icon_spacing = vie_styles[:support_style][:height] + margin * 2
  tool_style = { color: svg_color, style: support_style, spacing: icon_spacing, margin: margin }
  tools_list.each_with_index do |tool, index|
    add_icons_to_tool_zone(tool, index, tool_style)
  end
end

def create_matrix(id)
  center = grab(:center)
  matrix_style = vie_styles[:matrix_style].merge(id: "matrix_cell_#{id}", cells: {
                                                   particles: { margin: 9, smooth: 3, color: vie_styles[:cell_color],
                                                                shadow: { blur: 9, left: 3, top: 3,
                                                                          id: :default_cell_shadow,
                                                                          red: 0, green: 0, blue: 0, alpha: 0.3 } }
                                                 })

  modules = center.matrix(matrix_style)
  modules.right(0)

  # we return the matrix
  modules
end

# now the fill the U.I.

def set_title
  project_name = grab(:current_project).data
  grab(:title).data(project_name)
end
