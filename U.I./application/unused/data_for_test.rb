# # frozen_string_literal: true
# def project_list
#   {
#     i1: { name: "the song", data: :item1, action: :load_project },
#     i2: { name: "mon projet2", data: :item2, action: :load_project },
#     i3: { name: "autre projet", data: :item3, action: :load_project },
#     i4: { name: "my great song", data: :item4, action: :load_project }
#   }
# end
#
# def tool_list
#   {
#     t1: { id: :t1, action: :insert_module, label: :saw, type: :oscillator, icon: :'wave-saw' },
#     t2: { id: :t2, action: :insert_module, label: :sin, type: :oscillator, icon: :'wave-sine' },
#     t3: { id: :t3, action: :insert_module, label: :square, type: :oscillator, icon: :'wave-square' },
#     t4: { id: :t4, action: :insert_module, label: :triangle, type: :oscillator, icon: :'wave-triangle' },
#     t5: { id: :t5, action: :insert_module, label: 'audio in', type: :io, icon: :microphone },
#     t6: { id: :t6, action: :insert_module, label: 'audio out', type: :io, icon: :speaker },
#     t7: { id: :t7, action: :insert_module, label: 'midi in', type: :io, icon: :midi_in },
#     t8: { id: :t8, action: :insert_module, label: 'midi outdrdr', type: :io, icon: :midi_out },
#     t9: { id: :t9, action: :insert_module, label: 'sample', type: :waveform, icon: :waveform },
#     t10: { id: :t10, action: :insert_module, label: 'lowpass', type: :filter, icon: :low_pass },
#     t11: { id: :t11, action: :insert_module, label: 'highpass', type: :filter, icon: :high_pass },
#     t12: { id: :t12, action: :insert_module, label: 'bandpass', type: :io, icon: :band_pass },
#   }
# end
#
# # def receive_from_controller
# #   `
# #
# #   if (window.webkit) {
# # // write code here:
# #     } else {
# #      window.chrome.webview.addEventListener('message', arg => {
# #      console.log("data received from the controller : "+arg.data);
# #        });
# #     }
# # `
# #
# # end
# # receive_from_controller
#
#
# # JSON example
#
# vie_object_format = {
#   "modules": [
#     {
#       "id": "90be7add-12fd-476e-a267-c4ea65cd0bad",
#       "name": "Midi in",
#       "type_id": "midi",
#       "active": "true",
#       "input_slots": [
#         {
#           "id": "948151e4-93c5-428f-a2e9-01f560592a7d",
#           "name": "active",
#           "unit": "",
#           "value": 1
#         }
#       ],
#       "output_slots": [
#         {
#           "id": "394119f0-886e-4fae-8ae7-d3b187266760",
#           "name": "Midi out",
#           "unit": ""
#         }
#       ],
#       "position": {
#         "x": 0,
#         "y": 0,
#         "z": 0
#       }
#     },
#     {
#       "id": "82a15e17-c93a-4db1-af8a-d79a32c5b590",
#       "name": "Sin",
#       "type_id": "oscillator",
#       "active": "true",
#       "input_slots": [
#         {
#           "id": "f8ba775f-7a30-4728-99df-a6b6edf2666e",
#           "name": "active",
#           "unit": "",
#           "value": 1
#         },
#         {
#           "id": "002a39c1-e0eb-4ef3-9698-b8bc63faf467",
#           "name": "freq",
#           "unit": "khz",
#           "value": 4.40
#         }
#       ],
#       "output_slots": [
#         {
#           "id": "3d3dd92b-5a61-43bd-b15b-7356224f0fbc",
#           "name": "amplitude",
#           "unit": "db"
#         }
#       ],
#       "position": {
#         "x": 1,
#         "y": 0,
#         "z": 0
#       }
#     },
#     {
#       "id": "6f214581-52ba-484a-a896-1e137305bdd5",
#       "name": "mix",
#       "type_id": "mixer",
#       "active": "true",
#       "input_slots": [
#         {
#           "id": "fbf3a30a-f2a4-49ef-b56c-2830ad5f8917",
#           "name": "active",
#           "unit": "",
#           "value": 1
#         },
#         {
#           "id": "d1948f42-4260-4614-a3b8-2070b50c1e73",
#           "name": "input1",
#           "unit": "db",
#           "value": []
#         },
#         {
#           "id": "82399578-ec95-48f4-a909-d76e27571048",
#           "name": "input2",
#           "unit": "db",
#           "value": []
#         }
#       ],
#       "output_slots": [
#         {
#           "id": "a1f1cde0-5432-4465-b2a0-d06f759515b0",
#           "name": "mixout",
#           "unit": "db"
#         }
#       ],
#       "position": {
#         "x": 2,
#         "y": 0,
#         "z": 0
#       }
#     },
#     {
#       "id": "bf35dea9-f94e-4b73-9cdc-4c57c20ad996",
#       "name": "speak out",
#       "type_id": "output",
#       "active": "true",
#       "input_slots": [
#         {
#           "id": "fec34afa-a935-4892-949f-bb4ffc312468",
#           "name": "active",
#           "unit": "",
#           "value": 1
#         },
#         {
#           "id": "4b0fc5c3-fc04-4122-905d-aeebc4e06099",
#           "name": "input",
#           "unit": "db"
#         }
#       ],
#       "output_slots": [],
#       "position": {
#         "x": 3,
#         "y": 0,
#         "z": 0
#       }
#     }
#   ],
#   "links": [
#     {
#       "id": "dbde9cd8-462f-4f4a-80c6-cee0b0b9a5eb",
#       "sourceComponentId": "90be7add-12fd-476e-a267-c4ea65cd0bad",
#       "sourceOutputSlotId": "394119f0-886e-4fae-8ae7-d3b187266760",
#       "targetComponentId": "82a15e17-c93a-4db1-af8a-d79a32c5b590",
#       "targetInputSlotId": "002a39c1-e0eb-4ef3-9698-b8bc63faf467",
#       "active": "true"
#     },
#     {
#       "id": "0bf66202-50fe-4c82-81bd-7c828d1b4edb",
#       "sourceComponentId": "82a15e17-c93a-4db1-af8a-d79a32c5b590",
#       "sourceOutputSlotId": "3d3dd92b-5a61-43bd-b15b-7356224f0fbc",
#       "targetComponentId": "6f214581-52ba-484a-a896-1e137305bdd5",
#       "targetInputSlotId": "d1948f42-4260-4614-a3b8-2070b50c1e73",
#       "active": "true"
#     },
#     {
#       "id": "929faf01-6320-4273-8557-819f414c9d3a",
#       "sourceComponentId": "6f214581-52ba-484a-a896-1e137305bdd5",
#       "sourceOutputSlotId": "a1f1cde0-5432-4465-b2a0-d06f759515b0",
#       "targetComponentId": "bf35dea9-f94e-4b73-9cdc-4c57c20ad996",
#       "targetInputSlotId": "4b0fc5c3-fc04-4122-905d-aeebc4e06099",
#       "active": "true"
#     }
#   ]
# }
#
# json_test = vie_object_format.to_json

# hash_test = JSON.parse(json_test)

#
# # get_vie_module(json_test)
#
# def get_icon(icon_name)
#   load = <<STR
# <svg version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"   xml:space="preserve" viewBox="0 0 1024 1024">
#     <path id="folderCanvas-p1" stroke="none" fill="rgb(170, 170, 170)" d="M 848.62,922 L 186.38,922 C 125.42,922 76,871.39 76,808.99 L 76,243.87 C 76,181.5 125.42,130.89 186.38,130.89 L 344.58,130.89 344.05,130.89 C 386.9,130.89 425.87,155.91 443.98,195.05 L 466.73,243.89 848.62,243.89 C 909.58,243.89 959,294.5 959,356.92 L 959,808.99 C 959,871.39 909.58,922 848.62,922 Z M 149.58,461.65 L 149.58,808.99 C 149.58,829.79 166.07,846.65 186.38,846.65 L 848.62,846.65 C 868.93,846.65 885.42,829.79 885.42,808.99 L 885.42,461.65 149.58,461.65 Z M 149.58,386.29 L 885.42,386.29 885.42,356.92 C 885.42,336.11 868.93,319.25 848.63,319.25 L 443.92,319.25 444.15,319.25 C 429.87,319.25 416.89,310.91 410.85,297.87 L 378.06,227.7 378.02,227.62 C 371.99,214.58 359,206.24 344.72,206.24 L 186.38,206.24 C 166.07,206.24 149.58,223.1 149.58,243.89 L 149.58,386.29 Z M 149.58,386.29" />
# </svg>
#
# STR
#   clear = <<STR
# <svg class="svg-icon" style="width: 1em; height: 1em;vertical-align: middle;fill: currentColor;overflow: hidden;"
#      viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg">
#   <path id="path1" d="M772.096 368.64H654.336V153.6c0-78.848-63.488-142.336-142.336-142.336S369.664 74.752 369.664 153.6v215.04H251.904c-94.208 0-171.008 76.8-171.008 171.008v59.392c0 53.248 44.032 97.28 97.28 97.28h4.096l-51.2 121.856c-18.432 43.008-13.312 92.16 12.288 132.096 25.6 38.912 69.632 62.464 116.736 62.464h501.76c48.128 0 92.16-23.552 117.76-64.512 25.6-39.936 29.696-90.112 9.216-133.12L833.536 696.32h12.288c53.248 0 97.28-44.032 97.28-97.28v-59.392c0-95.232-76.8-171.008-171.008-171.008zM451.584 153.6c0-32.768 26.624-60.416 60.416-60.416 32.768 0 60.416 26.624 60.416 60.416v215.04H451.584V153.6zM808.96 904.192c-11.264 16.384-28.672 26.624-49.152 26.624h-501.76c-19.456 0-36.864-9.216-48.128-25.6s-12.288-35.84-5.12-54.272l63.488-150.528h12.288v124.928c0 22.528 18.432 40.96 40.96 40.96s40.96-18.432 40.96-40.96v-122.88-2.048h40.96v124.928c0 22.528 18.432 40.96 40.96 40.96s40.96-18.432 40.96-40.96v-122.88-3.072h40.96v125.952c0 22.528 18.432 40.96 40.96 40.96s40.96-18.432 40.96-40.96v-122.88-4.096h40.96v126.976c0 22.528 18.432 40.96 40.96 40.96s40.96-18.432 40.96-40.96v-122.88-5.12h14.336L815.104 849.92c6.144 16.384 5.12 36.864-6.144 54.272z m52.224-306.176c0 8.192-7.168 15.36-15.36 15.36H178.176c-8.192 0-15.36-7.168-15.36-15.36v-59.392c0-49.152 39.936-89.088 89.088-89.088h520.192c49.152 0 89.088 39.936 89.088 89.088v59.392z"
#         fill="#333C4F"/>
# </svg>
# STR
#   settings = <<STR
# <?xml version="1.0" encoding="UTF-8" standalone="no"?>
# <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
# <svg width="100%" height="100%" viewBox="0 0 800 800" version="1.1" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" xml:space="preserve" xmlns:serif="http://www.serif.com/" style="fill-rule:evenodd;clip-rule:evenodd;stroke-linecap:round;stroke-linejoin:round;stroke-miterlimit:10;">
#     <path id="settingsCanvas-settingsbezier" d="M464.37,112.09C477.93,150.24 521.5,168.28 558.07,150.9C616.12,123.31 676.69,183.88 649.1,241.93C631.72,278.5 649.76,322.07 687.91,335.63C748.48,357.17 748.48,442.83 687.91,464.37C649.76,477.93 631.72,521.5 649.1,558.07C676.69,616.12 616.12,676.69 558.07,649.1C521.5,631.72 477.93,649.76 464.37,687.91C442.83,748.48 357.17,748.48 335.63,687.91C322.07,649.76 278.5,631.72 241.93,649.1C183.88,676.69 123.31,616.12 150.9,558.07C168.28,521.5 150.24,477.93 112.09,464.37C51.52,442.83 51.52,357.17 112.09,335.63C150.24,322.07 168.28,278.5 150.9,241.93C123.31,183.88 183.88,123.31 241.93,150.9C278.5,168.28 322.07,150.24 335.63,112.09C357.17,51.52 442.83,51.52 464.37,112.09ZM400,300C390.8,300 381.9,301.24 373.44,303.57C331.1,315.2 300,353.97 300,400C300,455.23 344.77,500 400,500C455.23,500 500,455.23 500,400C500,344.77 455.23,300 400,300Z" style="fill:none;fill-rule:nonzero;stroke-width:60px;"/>
# </svg>
#
# STR
#   edition = <<STR
# <svg class="svg-icon" style="width: 1em; height: 1em;vertical-align: middle;fill: currentColor;overflow: hidden;"
#      viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg">
#   <path id="p1" d="M257.7 752c2 0 4-0.2 6-0.5L431.9 722c2-0.4 3.9-1.3 5.3-2.8l423.9-423.9c3.9-3.9 3.9-10.2 0-14.1L694.9 114.9c-1.9-1.9-4.4-2.9-7.1-2.9s-5.2 1-7.1 2.9L256.8 538.8c-1.5 1.5-2.4 3.3-2.8 5.3l-29.5 168.2c-1.9 11.1 1.5 21.9 9.4 29.8 6.6 6.4 14.9 9.9 23.8 9.9z m67.4-174.4L687.8 215l73.3 73.3-362.7 362.6-88.9 15.7 15.6-89zM880 836H144c-17.7 0-32 14.3-32 32v36c0 4.4 3.6 8 8 8h784c4.4 0 8-3.6 8-8v-36c0-17.7-14.3-32-32-32z"/>
# </svg>
# STR
#   select = <<STR
# <svg class="svg-icon" style="width: 1em; height: 1em;vertical-align: middle;fill: currentColor;overflow: hidden;"
#      viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg">
#   <path id="path" d="M281.6 137.813333a128 128 0 0 0-128 128v469.333334a128 128 0 0 0 128 128h213.333333a42.666667 42.666667 0 1 0 0-85.333334h-213.333333a42.666667 42.666667 0 0 1-42.666667-42.666666v-469.333334a42.666667 42.666667 0 0 1 42.666667-42.666666h469.333333a42.666667 42.666667 0 0 1 42.666667 42.666666v213.333334a42.666667 42.666667 0 1 0 85.333333 0v-213.333334a128 128 0 0 0-128-128h-469.333333z m294.613333 380.544l42.154667 365.184a21.333333 21.333333 0 0 0 38.357333 10.24l80.213334-108.629333 114.048 169.941333a42.666667 42.666667 0 1 0 70.826666-47.573333l-114.048-169.941333 124.416-36.693334a21.333333 21.333333 0 0 0 3.498667-39.552l-328.746667-164.522666a21.333333 21.333333 0 0 0-30.72 21.546666z"/>
# </svg>
# STR
#   group = <<STR
# <svg width="24" height="24" fill="none" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
#   <path id="p1" d="M5 7a2 2 0 1 0 0-4 2 2 0 0 0 0 4ZM19 7a2 2 0 1 0 0-4 2 2 0 0 0 0 4ZM21 19a2 2 0 1 1-4 0 2 2 0 0 1 4 0ZM5 21a2 2 0 1 0 0-4 2 2 0 0 0 0 4Z"
#         fill="#212121"/>
#   <path id="p2" d="M7.83 6a2.995 2.995 0 0 0 0-2h4.67A2.5 2.5 0 0 1 15 6.5V9h2.5a2.5 2.5 0 0 1 2.5 2.5v4.67a2.997 2.997 0 0 0-2 0V11.5a.5.5 0 0 0-.5-.5H15v1.5a2.5 2.5 0 0 1-2.5 2.5H11v2.5a.5.5 0 0 0 .5.5h4.67a2.997 2.997 0 0 0 0 2H11.5A2.5 2.5 0 0 1 9 17.5V15H6.5A2.5 2.5 0 0 1 4 12.5V7.83a2.995 2.995 0 0 0 2 0v4.67a.5.5 0 0 0 .5.5H9v-1.5A2.5 2.5 0 0 1 11.5 9H13V6.5a.5.5 0 0 0-.5-.5H7.83ZM13 12.5V11h-1.5a.5.5 0 0 0-.5.5V13h1.5a.5.5 0 0 0 .5-.5Z"
#         fill="#212121"/>
# </svg>
# STR
#   copy = <<STR
# <svg class="svg-icon" style="width: 1em; height: 1em;vertical-align: middle;fill: currentColor;overflow: hidden;"
#      viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg">
#   <path id="p1" d="M672 832 224 832c-52.928 0-96-43.072-96-96L128 160c0-52.928 43.072-96 96-96l448 0c52.928 0 96 43.072 96 96l0 576C768 788.928 724.928 832 672 832zM224 128C206.368 128 192 142.368 192 160l0 576c0 17.664 14.368 32 32 32l448 0c17.664 0 32-14.336 32-32L704 160c0-17.632-14.336-32-32-32L224 128z"/>
#   <path id="p2" d="M800 960 320 960c-17.664 0-32-14.304-32-32s14.336-32 32-32l480 0c17.664 0 32-14.336 32-32L832 256c0-17.664 14.304-32 32-32s32 14.336 32 32l0 608C896 916.928 852.928 960 800 960z"/>
#   <path id="p3" d="M544 320 288 320c-17.664 0-32-14.336-32-32s14.336-32 32-32l256 0c17.696 0 32 14.336 32 32S561.696 320 544 320z"/>
#   <path id="p4" d="M608 480 288.032 480c-17.664 0-32-14.336-32-32s14.336-32 32-32L608 416c17.696 0 32 14.336 32 32S625.696 480 608 480z"/>
#   <path id="p5" d="M608 640 288 640c-17.664 0-32-14.304-32-32s14.336-32 32-32l320 0c17.696 0 32 14.304 32 32S625.696 640 608 640z"/>
# </svg>
# STR
#   paste = <<STR
# <svg class="svg-icon" style="width: 1em; height: 1em;vertical-align: middle;fill: currentColor;overflow: hidden;"
#      viewBox="0 0 1024 1024" version="1.1" xmlns="http://www.w3.org/2000/svg">
#   <path id="path1" d="M810.666667 85.333333l-178.56 0c-17.493333-49.493333-64.426667-85.333333-120.106667-85.333333s-102.613333 35.84-120.106667 85.333333l-178.56 0c-47.146667 0-85.333333 38.186667-85.333333 85.333333l0 682.666667c0 47.146667 38.186667 85.333333 85.333333 85.333333l597.333333 0c47.146667 0 85.333333-38.186667 85.333333-85.333333l0-682.666667c0-47.146667-38.186667-85.333333-85.333333-85.333333zM512 85.333333c23.466667 0 42.666667 18.986667 42.666667 42.666667s-19.2 42.666667-42.666667 42.666667-42.666667-18.986667-42.666667-42.666667 19.2-42.666667 42.666667-42.666667zM810.666667 853.333333l-597.333333 0 0-682.666667 85.333333 0 0 128 426.666667 0 0-128 85.333333 0 0 682.666667z"/>
# </svg>
# STR
#   undo = <<STR
# <svg xmlns="http://www.w3.org/2000/svg"
#      viewBox="0 0 512 512"><!-- Font Awesome Free 5.15.4 by @fontawesome - https://fontawesome.com License - https://fontawesome.com/license/free (Icons: CC BY 4.0, Fonts: SIL OFL 1.1, Code: MIT License) -->
#   <path id="p" d="M255.545 8c-66.269.119-126.438 26.233-170.86 68.685L48.971 40.971C33.851 25.851 8 36.559 8 57.941V192c0 13.255 10.745 24 24 24h134.059c21.382 0 32.09-25.851 16.971-40.971l-41.75-41.75c30.864-28.899 70.801-44.907 113.23-45.273 92.398-.798 170.283 73.977 169.484 169.442C423.236 348.009 349.816 424 256 424c-41.127 0-79.997-14.678-110.63-41.556-4.743-4.161-11.906-3.908-16.368.553L89.34 422.659c-4.872 4.872-4.631 12.815.482 17.433C133.798 479.813 192.074 504 256 504c136.966 0 247.999-111.033 248-247.998C504.001 119.193 392.354 7.755 255.545 8z"/>
# </svg>
# STR
#
#   icons={
#     load: load,
#     clear: clear,
#     settings: settings,
#     edition: edition,
#     select: select,
#     group: group,
#     copy: copy,
#     paste: paste,
#     undo: undo
#
#   }
#   icons[icon_name]
# end
#
#
#
#


