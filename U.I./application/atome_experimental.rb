# frozen_string_literal: true
# class Atome
#
#   def particle_to_remove_decision(particle_to_remove)
#     if particle_to_remove.instance_of? Hash
#       particle_to_remove.each do |particle_found, value|
#         send("remove_#{particle_found}", value)
#       end
#     else
#       send(particle_to_remove, 0)
#     end
#   end
#
# end


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

# class Atome
#   def []=(params, value)
#     self.value[params] = value
#   end
# end
