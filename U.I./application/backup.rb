# frozen_string_literal: true

# frozen_string_literal: true

def svg_fetch(svg_name, width = 33, height = 33, svg_color = :lightgray, target)

  `
 fetch("./medias/images/icons/" +#{svg_name} +".svg")
    .then(response => response.text())
   .then(svgText => {
        let svgContainer = document.getElementById(#{target});
        let parser = new DOMParser();
        let svgDoc = parser.parseFromString(svgText, "image/svg+xml");
        let importedSVG = svgDoc.getElementsByTagName("svg")[0];
        importedSVG.style.width = #{width} + "px";
        importedSVG.style.height = #{height} + "px";
        let elements = importedSVG.getElementsByTagName("path");
        Array.from(elements).forEach(el => {
            el.setAttribute("fill", #{svg_color});
            el.setAttribute("stroke", #{svg_color});
        });
        svgContainer.appendChild(importedSVG);
    });

`
end

class Atome
  def []=(params, value)
    self.value[params] = value
  end
end

def vie_styles
  action_height = 60
  toolbox_with = 60
  inspector_with = 120
  navigation_height = 30
  {
    action_height: 60,
    toolbox_with: 60,
    inspector_with: inspector_with,
    navigation_height: 30,
    text_color: { red: 0.6, green: 0.6, blue: 0.6, id: :text_color },
    dark_color: { red: 0.1, green: 0.1, blue: 0.1, id: :action_color },
    darker_color2: { red: 0.127, green: 0.127, blue: 0.127, id: :darker_color2 },
    darker_color: { red: 0.127, green: 0.127, blue: 0.127, id: :darker_color },
    cell_color: { red: 0.127, green: 0.127, blue: 0.127 },
    cell_shadow: { blur: 9, left: 3, top: 3, id: :cell_shadow, red: 0, green: 0, blue: 0, alpha: 0.6 },
    connector: { id: :connector, right: 0, left: :auto, top: 0, width: 150, bottom: 0, height: :auto, attached: [:action_color, :cell_shadow] },
    toolbox_style: { id: :toolbox, left: 0, top: 0, width: toolbox_with, bottom: 0, height: :auto, attached: [:action_color] },
    action_style: { id: :action, left: toolbox_with, top: 0, width: :auto, right: 0, height: action_height, attached: [:action_color] },
    inspector_style: { id: :inspector, left: toolbox_with, top: 0, width: inspector_with, bottom: 0, height: :auto, attached: [:action_color] },
    filer: { id: :filer, top: 12, bottom: 36, left: 6, width: inspector_with - 12, overflow: :auto, height: :auto, attached: [:darker_color], smooth: 6 },
    navigation_style: { id: :navigation, left: 0, bottom: 0, top: :auto, width: :auto, right: 0, height: navigation_height, attached: [:action_color] },
    center_style: { id: :center, left: toolbox_with + inspector_with, bottom: navigation_height, top: action_height, right: 0, width: :auto, height: :auto },
  }
end
# this the atome where we store the connection running
element(id: :connection, data: [1, 3])


color(vie_styles[:dark_color])
color(vie_styles[:darker_color])
color(vie_styles[:darker_color2])
color(vie_styles[:text_color])
shadow(vie_styles[:cell_shadow])
grab(:view).attached([:action_color])
center = box(vie_styles[:center_style])
action = box(vie_styles[:action_style])
toolbox = box(vie_styles[:toolbox_style])
inspector = box(vie_styles[:inspector_style])
navigation = box(vie_styles[:navigation_style])

modules = center.matrix({ id: :modules, top: 0, left: 0, smooth: 0, columns: { count: 8 }, rows: { count: 8 },
                          color: { alpha: 0 },
                          cells: { particles: { margin: 9, smooth: 3,
                                                color: vie_styles[:cell_color], shadow: vie_styles[:cell_shadow] } } })

# # add to reassign because of a bug
center.color(vie_styles[:dark_color])

if center.width.to_px > center.height.to_px
  modules.resize(center.height.to_px, center.height.to_px)
else
  modules.resize(center.width.to_px, center.width.to_px)
end

modules.width(:auto)
modules.right(0)
$window.on :resize do |e|
  if center.width.to_px > center.height.to_px
    modules.resize(center.height.to_px, center.height.to_px)
  else
    modules.resize(center.width.to_px, center.width.to_px)
  end
  modules.width(:auto)
  modules.right(0)
end

modules.cell(3).color(:orange)
modules.cell(3).touch(:long) do
  modules.cell(3).color(:red)
end

modules.cell(5).color(:yellowgreen)

modules.children.value.each_with_index do |module_found, index|
  modules.cell(index).touch(:down) do
    puts "modules #{index} touch down"

    grab(:connection).data[0] = index
    # alert grab(:connection).data
  end
  modules.cell(index).touch(:up) do
    grab(:connection).data[1] = index
    grab(:connector).delete(true) if grab(:connector)
    unless grab(:connection).data.value[0] == grab(:connection).data.value[1]
      connector=grab(:center).box(vie_styles[:connector])
      connector.text(data: grab(:connection).data.value)
    end


    # puts "modules #{index} touch up"
  end
end
# logo
action.image({ path: "./medias/images/logos/vie.svg", left: :auto, right: 3, top: 0, size: 66 })
# file
tool_zone = toolbox.box({ id: :tool_zone, top: 12, bottom: 36, left: 6, width: 50, overflow: :auto, height: :auto,
                          attached: [:darker_color2], smooth: 6, color: { red: 1, green: 1, blue: 1, alpha: 0.03 } })
# tool_zone.color(:orange)
# tool_zone.box({id: :yoyo})
# tool_zone.box({id: :yoyi})
# tool_zone.text({ data: :hello, id: :hello })

def fill_toolzone (ids)
  svg_color = :gray
  ids.each_with_index do |id_found, index|
    support = grab(:tool_zone).box({ width: 33, height: 33, top: (33 + 19) * index, left: 0 })
    svg_fetch(id_found, 33, 33, svg_color, support.id)
    support.touch(true) do
      send(id_found)
    end
  end
end

def list(parent, style, items)
  items.each_with_index do |(k, v), index|
    item = parent.text(style)
    item.top(25 * index)
    item.touch(true) do
      item.color(:lightgray)
    end
  end
end

inspector.box(vie_styles[:filer])

def clear_zone(zone)
  zone.children.each do |child_found|
    grab(child_found).delete(true)
  end
  zone.children = []
end

def hamburger
  filer = grab(:filer)
  clear_zone(filer)
  text_list_style = { visual: { size: 15 }, data: "mon projet", left: 9, top: 6, attached: [:text_color] }
  list(filer, text_list_style, {
    i1: { title: "mon projet", data: :item1, action: :my_action },
    i2: { title: "mon projet2", data: :item1, action: :my_action },
    i3: { title: "autre projet", data: :item1, action: :my_action },
    i4: { title: "my great song", data: :item1, action: :my_action },
  })
end

def settings
  filer = grab(:filer)
  clear_zone(filer)
end

def edit
  filer = grab(:filer)
  clear_zone(filer)
end

def low_pass
  filer = grab(:filer)
  clear_zone(filer)
end

fill_toolzone([:hamburger, :settings, :edit, :low_pass])
# alert filer
# grab(:tool_zone).image({ path: "./medias/images/icons/load.svg", top: 15, left: 0, size: 24 })

# files.touch(true) do
#   grab(:filer).color(:red)
# end
# module
# filer.image({ path: "./medias/images/icons/modules.svg", top: 63, left: 15, size: 24 })

# edit
# filer.image({ path: "./medias/images/icons/edit.svg", top: :auto, bottom: 15, left: 23, size: 24 })
# filer.text({ data: "Mon projetlkjlkjl", top: :auto, bottom: 15, left: 23, size: 24 })
# file_list=inspector
# file_list.text({visual: {size:  15 }, data: "mon projet", left: 9, top: 6, attached: [txt_color.id]})
# file_list.text({visual: {size:  15 }, data: "mon projet", left: 9, top: 36, attached: [txt_color.id]})
# file_list.text({visual: {size:  15 }, data: "mon projet", left: 9, top: 66, attached: [txt_color.id]})
# module_list=box.box(list_style)
# tool_list=box.box(list_style)
# listing = inspector.box(list_style)
#
# def list(parent, style, items)
#   items.each_with_index do |(k, v), index|
#     item = parent.text(style)
#     item.top(25 * index)
#     item.touch(true) do
#       item.color(:lightgray)
#     end
#   end
# end
#
# text_list_style = { visual: { size: 15 }, data: "mon projet", left: 9, top: 6, attached: [txt_color.id] }
# list(listing, text_list_style, {
#   i1: { title: "mon projet", data: :item1, action: :my_action },
#   i2: { title: "mon projet2", data: :item1, action: :my_action },
#   i3: { title: "autre projet", data: :item1, action: :my_action },
#   i4: { title: "my great song", data: :item1, action: :my_action },
# })

class WebObject

  def initialize(object)
    @webobject = object
  end

  def style
    # `#{@webobject}.style`
  end

  def web_object
    @webobject
  end

end

class Atome
  def webobject
    web_object = `document.getElementById(#{id.value})`
    WebObject.new(web_object)
    # web_object
  end

end

a = center.webobject
a.style
# `#{a}.style.left='500px'`

# FIXME : impossible to create more than one matrix on a single page
# FIXME : when adding a matrix the parent get its color
# FIXME : when adding a shadow to matrix cells the parent of the parent get the shadow too

# TODO : text should be define in 'vw' to resize according to it's parent
# TODO : Children can't be removed , idem for attach
# TODO : remmove doesn't work with shadow

# class Myclass
#   def initialize(params)
#     @my_val = params
#   end
#   def method_missing(name, *args)
#     @my_val
#   end
# end
#
# a = Myclass.new(:hello)
# b=a.upcase
# puts b
# comment puis je recuperer @my_val soit : :hello, quand je fais un puts a?
############################# svg below
# b = box({ id: :the_box })

# svg_color = :cyan
# adress = 'stop'
# svg_fetch(adress,33,33,svg_color, :tool_zone)