# frozen_string_literal: true
def tools_list
  {
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
end
# # logo
# logo_color ={ red: 0.38, green: 1, blue: 0}
# logo = <<~STR
#   <path id="vieCanvas-bezier" stroke="rgb(0, 0, 0)" stroke-width="1" stroke-miterlimit="10" fill="rgb(255, 0, 0)" d="M 73.04,26.41 C 50.57,12.14 15.77,53.39 15.81,85.33 15.83,107.68 23.49,124.45 35.37,139.38 97.06,203.55 73.1,232.52 109.61,231.71 134.09,231.16 181.15,134.57 220.5,138.31 232.63,123.3 240.52,106.7 240.5,85.07 240.5,84.51 240.49,83.95 240.47,83.4 211.52,29.92 146.74,182.8 114.45,179.38 69.64,174.65 90.68,37.61 73.04,26.41 Z M 172.32,76.13 C 172.32,94.63 157.34,109.64 138.85,109.64 120.36,109.64 105.37,94.63 105.37,76.13 105.37,57.62 120.36,42.62 138.85,42.62 157.34,42.62 172.32,57.62 172.32,76.13 Z M 172.32,76.13" />
# STR
#
# logo2=grab(:action).vector({ width: 133, height: 133, left: :auto, top: 7, right: 15,id: :logo, definition: logo, color: logo_color})


# Basic U.I. elements
box(vie_styles[:center_style]) # the playground,( where the matrix lies)
box(vie_styles[:action_style]) # the action zone (the top bar)
box(vie_styles[:toolbox_style]) # the toolbox zone at the left side of the screen
box(vie_styles[:inspector_style]) # the inspector where at the right of the tool zone

# this the atome used to store the tools in the toolbar
element(id: :tools, data: tools_list)
# this the atome used to store the module to be associated
element(id: :connection, data: [])
# this the atome used to store the selected module
element(id: :selected, data: [])
# this the atome used to store the selected module
element(id: :context, data: { mode: :default, event: :released })
# this the atome used to store the current project
element(id: :current_project)
# this the atome used to store the content for every matrix in the current project
element(id: :project_matrix, data: {})
# this the atome used to store the active matrix (the one the screen)
element(id: :active_matrix)
# this the atome used to store all available user projects
element(id: :debug, data: { debug: false })

def add_icons_to_tool_zone(tool, index, tool_style)
  tool_name = tool[0]

  svg_color = tool_style[:color]
  support_style = tool_style[:style]
  icon_spacing = tool_style[:spacing]
  margin = tool_style[:margin]

  # v=vector({id: support.id })
  # alert get_icon(tool_name)
  # alert svg_color
  # if tool_name== :load
  #   # tt=support.text(tool_name)
  #   # tt.left(0)
  #   # tt.top(0)
  #   # alert "#{support.id}: #{tool_name}"
  vector_data=get_icon(tool_name)

  # grab(tool).attached(:inactive_color)
  support = grab(:toolbox).vector(support_style.merge({ top: (icon_spacing * index) + margin,
                                                        id: tool_name}))

  support.definition(vector_data)
  support.attached(:inactive_color)
  #   # TODO: important for future use keep to code below to fetch items
  #   # display_svg(get_icon(tool_name), svg_color, support.id)
  #   support.color(:red)
  #   support.box({left: 0, top: 0, width: 22, height: 22})
  #   support.drag(true)
  # else
  #   # support.color(:yellow)
  #
  # end

  # tt.color(svg_color)


  # edition = <<~STR
  #     <path id="p1" d="M257.7 752c2 0 4-0.2 6-0.5L431.9 722c2-0.4 3.9-1.3 5.3-2.8l423.9-423.9c3.9-3.9 3.9-10.2 0-14.1L694.9 114.9c-1.9-1.9-4.4-2.9-7.1-2.9s-5.2 1-7.1 2.9L256.8 538.8c-1.5 1.5-2.4 3.3-2.8 5.3l-29.5 168.2c-1.9 11.1 1.5 21.9 9.4 29.8 6.6 6.4 14.9 9.9 23.8 9.9z m67.4-174.4L687.8 215l73.3 73.3-362.7 362.6-88.9 15.7 15.6-89zM880 836H144c-17.7 0-32 14.3-32 32v36c0 4.4 3.6 8 8 8h784c4.4 0 8-3.6 8-8v-36c0-17.7-14.3-32-32-32z"/>
  # STR
  # v= vector(definition: edition)
  # v.color(:red)
  # v.width(33)
  # v.height(33)
  # v.left(0)
  # v.top(0)
  # vector({id: support.id })
  # v.definition(edition)
  # alert get_icon(tool_name)
              # .definition(edition)

  options = tool[1]
  tool_events(support, tool_name, options)
end

def fill_tool_zone(tools_list)
  # the ids are used to retrieve the icon and look the the corresponding method when touch
  tool_base__color = {red: 0.8, green: 0.6, blue: 0.5}#'rgba(180, 120, 50, 1)'
  margin = vie_styles[:margin]
  support_style = vie_styles[:support_style]
  icon_spacing = vie_styles[:support_style][:height] + margin * 2
  tool_style = { color: tool_base__color, style: support_style, spacing: icon_spacing, margin: margin }
  tool_default_color = tool_style[:color]

  tool_color=color(tool_default_color)
  tools_list.each_with_index do |tool, index|
    # edition = <<~STR
    #   <path id="p1" d="M257.7 752c2 0 4-0.2 6-0.5L431.9 722c2-0.4 3.9-1.3 5.3-2.8l423.9-423.9c3.9-3.9 3.9-10.2 0-14.1L694.9 114.9c-1.9-1.9-4.4-2.9-7.1-2.9s-5.2 1-7.1 2.9L256.8 538.8c-1.5 1.5-2.4 3.3-2.8 5.3l-29.5 168.2c-1.9 11.1 1.5 21.9 9.4 29.8 6.6 6.4 14.9 9.9 23.8 9.9z m67.4-174.4L687.8 215l73.3 73.3-362.7 362.6-88.9 15.7 15.6-89zM880 836H144c-17.7 0-32 14.3-32 32v36c0 4.4 3.6 8 8 8h784c4.4 0 8-3.6 8-8v-36c0-17.7-14.3-32-32-32z"/>
    # STR
    # tool_name = tool[0]
    #
    #
    # support_style = tool_style[:style]
    # icon_spacing = tool_style[:spacing]
    # margin = tool_style[:margin]
    # definition_found=get_icon(tool_name)
    # t=grab(:toolbox).vector({top: index*39, left: 0, width: 33, height: 33,definition: definition_found, attached: tool_color.id })
    # t.touch(true) do
    #   alert :pol
    # end

    # grab(:toolbox).vector({ definition: edition, color: :red })
    add_icons_to_tool_zone(tool, index, tool_style)
  end
end

def build_matrix
  center = grab(:center)
  matrix_id="vie_matrix"
  matrix_style = vie_styles[:matrix_style].merge(id: matrix_id, cells: {
                                                   particles: { margin: 9, smooth: 3, color: vie_styles[:cell_color],
                                                                shadow: { blur: 9, left: 3, top: 3,
                                                                          id: :default_cell_shadow,
                                                                          red: 0, green: 0, blue: 0, alpha: 0.3 } }
                                                 })

  new_matrix = center.matrix(matrix_style)


  # we initialize the cells tags
  new_matrix.cells.each do |cell|
    grab(cell).tag({})
  end

  # we return the matrix
  new_matrix
end

# now the fill the U.I.

def set_title
  project_name = grab(:current_project).data
  grab(:title).data(project_name)
end



