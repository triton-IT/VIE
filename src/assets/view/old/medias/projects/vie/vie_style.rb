# styles

ref_size = 69
color_theme = {
  back_color: { red: 0.2,
                green: 0.2,
                blue: 0.2,
                alpha: 0.6 },
  base_color: { red: 0.15,
                green: 0.15,
                blue: 0.15,
                alpha: 1 },
  selected_color: { red: 0,
                    green: 0.9,
                    blue: 0.3,
                    alpha: 0.2 },
  flash_color: { red: 0.2,
                 green: 1,
                 blue: 0.6,
                 alpha: 0.9 },
  matrix_background: { red: 1,
                       green: 0.3,
                       blue: 0.3,
                       alpha: 0 },
  text_color: { red: 0.69,
                green: 0.69,
                blue: 0.69,
                alpha: 1 },

}

vie_style = {
  ref_size: ref_size,
  animation_duration: 0.3,
  matrix_background: color_theme[:matrix_background],
  matrix: {
    color: color_theme[:back_color]
  },
  cell: {
    color: color_theme[:base_color],
  },
  label: {
    color: color_theme[:text_color],
    visual: 12,
    center: true
  },
  cell_selected: {
    color: color_theme[:selected_color],
    shadow: { bounding: true },
    border: { color: :orange, thickness: 3, pattern: :solid }
  },
  flash_color: color_theme[:flash_color],
  lines: {
    height: 33,
    color: color_theme[:base_color]
  },
  alternated_lines: {
    height: 33,
    color: color_theme[:selected_color]
  },
  navigator: { width: ref_size, height: :auto, x: 0, y: ref_size, yy: ref_size, color: color_theme[:base_color], shadow: { bounding: true }, z: 1 },
  inspector: { width: ref_size, height: :auto, xx: 0, y: ref_size, yy: ref_size, color: color_theme[:base_color], shadow: { bounding: true }, z: 1 },
  details: { width: :auto, height: ref_size, x: 0, yy: 0, xx: 0, color: color_theme[:base_color], shadow: { bounding: true }, z: 2 },
  action: { width: :auto, height: ref_size, x: 0, y: 0, xx: 0, color: color_theme[:base_color], shadow: { bounding: true }, z: 2 },
  matrix_shell: { center: true,y: ref_size,yy: ref_size, xx: ref_size, color: :red, z: 0 },
  title: { width: 333, height: ref_size / 3, center: { axis: :x, dynamic: true }, y: 0, color: color_theme[:base_color], shadow: { bounding: true }, z: 3, smooth: [0, 0, 9, 9] },
  title_label: { visual: 15, y: 3, center: :x },
  module: { color: color_theme[:base_color] },
  module_selected: { color: color_theme[:selected_color],
                     shadow: { bounding: true } },

}
Atome.new({ atome_id: :vie_style, content: vie_style })
Atome.new({ atome_id: :color_theme, content: color_theme })
