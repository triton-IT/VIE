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
      send(code, item_id)
    end
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