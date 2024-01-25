# frozen_string_literal: true

require 'version'
require 'eVe_add_on'
require 'styles'
require 'sliders'
require 'tool_box'
console(true)

toolbox=box({id: :toolbox, top: 0, bottom: 0, height: :auto, width: '3%', left: 0})
inspector=box({id: :inspector, top: 0, bottom: 0, height: :auto, width: '3%', right: 0, left: :auto})
contextual=box({id: :infos, top: 0,  height:'3%', width: :auto, right: 0, left: 0})
details=box({id: :infos, bottom: 0, top: :auto, height:'12%', width: :auto, right: 0, left: 0})

light_box=matrix(center: true, width: 150, height: 120, color: :red)



# clear play paste mix


# class HTML
#
#   def table(data)
#     alert data.first.keys
#     table_html = JS.global[:document].createElement("table")
#
#     # Crer colgroup
#     colgroup = JS.global[:document].createElement("colgroup")
#     data.first.keys.each do |key|
#       col = JS.global[:document].createElement("col")
#       # Ajouter des styles ou des classes spcifiques  chaque col ici
#       col.setAttribute('class', key.to_s)
#       colgroup.appendChild(col)
#     end
#     table_html.appendChild(colgroup)
#
#     thead = JS.global[:document].createElement("thead")
#     header_row = JS.global[:document].createElement("tr")
#     header_row.setAttribute('id', "0")
#     data.first.keys.each do |header|
#       th = JS.global[:document].createElement("th")
#       th.textContent = header
#       header_row.appendChild(th)
#     end
#     thead.appendChild(header_row)
#     table_html.appendChild(thead)
#
#     tbody = JS.global[:document].createElement("tbody")
#     data.each_with_index do |row, row_index|
#       tr = JS.global[:document].createElement("tr")
#       tr.setAttribute('id', row_index.to_s)
#       row.values.each_with_index do |cell, cell_index|
#         td = JS.global[:document].createElement("td")
#         td.setAttribute('id', "#{row_index}_#{cell_index}")
#         td.style[:border] = "1px solid black"
#         td.style[:overflow] = "hidden" # Modifi pour cacher le contenu dbordant
#         td.style[:backgroundColor] = "white"
#         td.style[:boxShadow] = "10px 10px 5px #888888"
#
#         # Hauteur fixe pour toutes les cellules
#         # td.style[:height] = '50px' # Hauteur fixe, par exemple 50px
#
#         if cell.instance_of? Atome
#           html_element = JS.global[:document].getElementById(cell.id.to_s)
#           td.appendChild(html_element)
#
#           # Styles pour contenir l'lment Atome dans la cellule
#           html_element.style[:height] = '55px'
#           alert html_element.style[:height]
#           # html_element.style[:overflow] = 'hidden'
#           # html_element.style[:objectFit] = 'cover'
#
#           # Positionnement de l'lment Atome
#           html_element.style[:transformOrigin] = "top left"
#           html_element.style[:position] = "relative"
#           cell.top(0)
#           cell.left(0)
#         else
#           td.textContent = cell
#         end
#
#         # Largeur des cellules (inchange)
#         td.style[:width] = '33px'
#         tr.appendChild(td)
#       end
#       tbody.appendChild(tr)
#     end
#     table_html.appendChild(tbody)
#     JS.global[:document].querySelector("##{@id}").appendChild(table_html)
#   end
#
# end
#
# new({ atome: :matrix })
# new({ particle: :format })
# new({ particle: :border })
# new({ particle: :cells }) do |value, user_proc|
#   user_proc.call
# end
# new({ particle: :rows })
# new({ particle: :columns })
# new({ method: :border, type: :hash, renderer: :html }) do |value, _user_proc|
#   thickness = value[:thickness] || 5
#   type = value[:pattern] || :solid
#   color = if value[:color]
#             color_found = value[:color]
#             "#{color_found.red * 255},#{color_found.green * 255},#{color_found.blue * 255},#{color_found.alpha} "
#           else
#             "0,0,0,1"
#           end
#
#   html.style(:border, "#{type} #{thickness}px rgba(#{color})")
# end
#
# new({ method: :data, type: :string, specific: :matrix, renderer: :html }) do |value, _user_proc|
#   html.table(value)
# end
#
# c = circle({ id: :my_cirle, color: :red, drag: true })
# c.box
# c.touch(true) do
#   alert :okk
# end
# m = matrix({ renderers: [:html], attach: :view, id: :my_test_box, type: :matrix, apply: [:shape_color],
#              left: 333, top: 0, width: 300, smooth: 15, height: 900, overflow: :scroll, data: [
#     { dfgdf: 1, name: 'Alice', age: 30, no: 'oko', t: 123, r: 654, f: 123, g: 654, w: 123, x: 654, c: 123, v: 654 },
#     { id: 2, name: 'Bob', age: 22 },
#     { dfg: 3, name: 'toto', age: 18, no: grab(:my_cirle) },
#     { dfgd: 3, name: 'tutu', age: 18, no: image(:red_planet) }
#
#   ]
#            })
# m.color(:orange)
# m.cells({}) do |params|
#   alert :cellingd
# end
# m.rows({}) do |params|
#
# end
# m.columns({}) do |params|
#
# end
# m.add({column: {fffb: {  name: 'tata', age: 18, no: grab(:my_cirle)}}})
#
# m.border({ thickness: 5, color: color(:blue), pattern: :dotted })

# console(true)