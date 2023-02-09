# frozen_string_literal: true
def project_list
  {
    i1: { name: "the song", data: :item1, action: :load_project },
    i2: { name: "mon projet2", data: :item2, action: :load_project },
    i3: { name: "autre projet", data: :item3, action: :load_project },
    i4: { name: "my great song", data: :item4, action: :load_project }
  }
end

def tool_list
  {
     t1: {id: :t1, action: :insert_module, label: :saw, type: :oscillator, icon: :'wave-saw' },
     t2: {id: :t2 ,action: :insert_module, label: :sin, type: :oscillator, icon: :'wave-sine' },
     t3: {id: :t3 ,action: :insert_module, label: :square, type: :oscillator, icon: :'wave-square' },
     t4: {id: :t4 ,action: :insert_module, label: :triangle, type: :oscillator, icon: :'wave-triangle' },
     t5: {id: :t5 ,action: :insert_module, label: 'audio in', type: :io, icon: :microphone },
     t6: {id: :t6 ,action: :insert_module, label: 'audio out', type: :io, icon: :speaker },
     t7: {id: :t7 ,action: :insert_module, label: 'midi in', type: :io, icon: :midi_in },
     t8: {id: :t8 ,action: :insert_module, label: 'midi outdrdr', type: :io, icon: :midi_out },
     t9: {id: :t9 ,action: :insert_module, label: 'sample', type: :waveform, icon: :waveform },
    t10: {id: :t10 ,action: :insert_module, label: 'lowpass', type: :filter, icon: :low_pass },
    t11: {id: :t11 ,action: :insert_module, label: 'highpass', type: :filter, icon: :high_pass },
    t12: {id: :t12 ,action: :insert_module, label: 'bandpass', type: :io, icon: :band_pass },
  }
end

# JSON example

vie_object_format = {
  "modules": [
    {
      "id": "90be7add-12fd-476e-a267-c4ea65cd0bad",
      "name": "Midi in",
      "type_id": "midi",
      "active": "true",
      "input_slots": [
        {
          "id": "948151e4-93c5-428f-a2e9-01f560592a7d",
          "name": "active",
          "unit": "",
          "value": 1
        }
      ],
      "output_slots": [
        {
          "id": "394119f0-886e-4fae-8ae7-d3b187266760",
          "name": "Midi out",
          "unit": ""
        }
      ],
      "position": {
        "x": 0,
        "y": 0,
        "z": 0
      }
    },
    {
      "id": "82a15e17-c93a-4db1-af8a-d79a32c5b590",
      "name": "Sin",
      "type_id": "oscillator",
      "active": "true",
      "input_slots": [
        {
          "id": "f8ba775f-7a30-4728-99df-a6b6edf2666e",
          "name": "active",
          "unit": "",
          "value": 1
        },
        {
          "id": "002a39c1-e0eb-4ef3-9698-b8bc63faf467",
          "name": "freq",
          "unit": "khz",
          "value": 4.40
        }
      ],
      "output_slots": [
        {
          "id": "3d3dd92b-5a61-43bd-b15b-7356224f0fbc",
          "name": "amplitude",
          "unit": "db"
        }
      ],
      "position": {
        "x": 1,
        "y": 0,
        "z": 0
      }
    },
    {
      "id": "6f214581-52ba-484a-a896-1e137305bdd5",
      "name": "mix",
      "type_id": "mixer",
      "active": "true",
      "input_slots": [
        {
          "id": "fbf3a30a-f2a4-49ef-b56c-2830ad5f8917",
          "name": "active",
          "unit": "",
          "value": 1
        },
        {
          "id": "d1948f42-4260-4614-a3b8-2070b50c1e73",
          "name": "input1",
          "unit": "db",
          "value": []
        },
        {
          "id": "82399578-ec95-48f4-a909-d76e27571048",
          "name": "input2",
          "unit": "db",
          "value": []
        }
      ],
      "output_slots": [
        {
          "id": "a1f1cde0-5432-4465-b2a0-d06f759515b0",
          "name": "mixout",
          "unit": "db"
        }
      ],
      "position": {
        "x": 2,
        "y": 0,
        "z": 0
      }
    },
    {
      "id": "bf35dea9-f94e-4b73-9cdc-4c57c20ad996",
      "name": "speak out",
      "type_id": "output",
      "active": "true",
      "input_slots": [
        {
          "id": "fec34afa-a935-4892-949f-bb4ffc312468",
          "name": "active",
          "unit": "",
          "value": 1
        },
        {
          "id": "4b0fc5c3-fc04-4122-905d-aeebc4e06099",
          "name": "input",
          "unit": "db"
        }
      ],
      "output_slots": [],
      "position": {
        "x": 3,
        "y": 0,
        "z": 0
      }
    }
  ],
  "links": [
    {
      "id": "dbde9cd8-462f-4f4a-80c6-cee0b0b9a5eb",
      "sourceComponentId": "90be7add-12fd-476e-a267-c4ea65cd0bad",
      "sourceOutputSlotId": "394119f0-886e-4fae-8ae7-d3b187266760",
      "targetComponentId": "82a15e17-c93a-4db1-af8a-d79a32c5b590",
      "targetInputSlotId": "002a39c1-e0eb-4ef3-9698-b8bc63faf467",
      "active": "true"
    },
    {
      "id": "0bf66202-50fe-4c82-81bd-7c828d1b4edb",
      "sourceComponentId": "82a15e17-c93a-4db1-af8a-d79a32c5b590",
      "sourceOutputSlotId": "3d3dd92b-5a61-43bd-b15b-7356224f0fbc",
      "targetComponentId": "6f214581-52ba-484a-a896-1e137305bdd5",
      "targetInputSlotId": "d1948f42-4260-4614-a3b8-2070b50c1e73",
      "active": "true"
    },
    {
      "id": "929faf01-6320-4273-8557-819f414c9d3a",
      "sourceComponentId": "6f214581-52ba-484a-a896-1e137305bdd5",
      "sourceOutputSlotId": "a1f1cde0-5432-4465-b2a0-d06f759515b0",
      "targetComponentId": "bf35dea9-f94e-4b73-9cdc-4c57c20ad996",
      "targetInputSlotId": "4b0fc5c3-fc04-4122-905d-aeebc4e06099",
      "active": "true"
    }
  ]
}




json_test = vie_object_format.to_json
# alert "#{json_test.class }: #{json_test.class}"
hash_test = JSON.parse(json_test)
# alert "#{hash_test.class} : #{hash_test}"

# get_vie_module(json_test)

