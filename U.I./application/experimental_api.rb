# frozen_string_literal: true

Essentials.new_default_params({ list: { colors: %i[red blue], items: { color: :black, size: 33 },
                                        sort: :alphabetically, height: 60, width: :auto, margin: 0,
                                        spacing: 3, view: :data, listing: {} } })

new({ particle: :list })

new ({ post: :list }) do |params|
  list_style = Essentials.default_params[:list].merge(params[:style])

  list_code = params[:code]
  list_listing = params[:listing]

  line_height = list_style[:height]
  line_width = list_style[:width]
  line_margin = list_style[:margin]
  line_spacing = list_style[:spacing]
  # colors = list_style[:colors]
  list_listing.each_with_index do |item, index|
    support = box({ top: (line_height + line_spacing) * index, height: line_height, width: line_width, left: line_margin, right: line_margin })
    support.text({ data: item[:label], visual: { size: 12 } })
    code = item[:code] ||= list_code
    item_id = item[:id]

    support.touch(true) do
      # alert :poeyr
      send(code, item_id)
    end
  end

end

class Atome

  def initialize(atomes = {}, &atomes_proc)

    atomes.each_value do |elements|

      if  Universe.atomes.key?(elements[:id])
        puts "The id #{elements[:id]} is already taken, you must change it"
        # `throw new Error("this id is already exist, you must change it");`
      else
        # the instance variable below contain the id all any atomes that need to be informed when changes occurs
        @broadcast = {}
        # # now we store the proc in a an atome's property called :bloc
        elements[:code] = atomes_proc if atomes_proc

        @atome = elements
        # we initiate the rendering suing set_type method,
        # eg for for browser we will call :browser_type generate method in identity.rb file
        collapse
      end
    end
  end
  def duplicate(params={})
    if params==true
      params={}
    end
    # properties=atome[]
    properties=  self.atome.dup
    properties.delete(:id)
    atome_type= properties.delete(:type)
    new_id= params[:id] ||= identity_generator(atome_type)
    properties={type: atome_type}.merge(properties)
    properties={id: new_id}.merge(properties)
    properties={renderers: properties}.merge(properties)
    properties.delete(:color)
    params[:remove].each do |particle_to_remove|
      # puts particle_to_remove
      properties.delete(particle_to_remove)
    end
    # properties[:left]=333
    Atome.new({atome_type => properties})
    # duplication=Atome.new({atome_type => properties})
    # duplication
  end



end

def svg_fetch(svg_name, svg_color = :lightgray, target)

  `
 fetch("./medias/images/icons/" +#{svg_name} +".svg")
    .then(response => response.text())
   .then(svgText => {
        let svgContainer = document.getElementById(#{target});
        let parser = new DOMParser();
        let svgDoc = parser.parseFromString(svgText, "image/svg+xml");
        let importedSVG = svgDoc.getElementsByTagName("svg")[0];
        importedSVG.style.width =  "100%";
        importedSVG.style.height =  "100%";
        let elements = importedSVG.getElementsByTagName("path");
        Array.from(elements).forEach(el => {
            el.setAttribute("fill", #{svg_color});
            el.setAttribute("stroke", #{svg_color});
        });
        svgContainer.appendChild(importedSVG);
    });

`
end


def display_svg(svg_content, svg_color = :lightgray, target)
  `
       let svgContainer = document.getElementById(#{target});
        let parser = new DOMParser();
        let svgDoc = parser.parseFromString(#{svg_content}, "image/svg+xml");
        let importedSVG = svgDoc.getElementsByTagName("svg")[0];
        importedSVG.style.width =  "100%";
        importedSVG.style.height =  "100%";
        let elements = importedSVG.getElementsByTagName("path");
        Array.from(elements).forEach(el => {
            el.setAttribute("fill", #{svg_color});
            el.setAttribute("stroke", #{svg_color});
        });
        svgContainer.appendChild(importedSVG);
`
end



# <div class="atome invisible_color" id="module_support_0" style="width: 23px; height: 23px; left: 6px; top: 9px; z-index: 5;"></div>
#
# <div class="atome invisible_color" id="module_support_1" style="width: 23px; height: 23px; left: 6px; top: 50px; z-index: 5;"> </div>