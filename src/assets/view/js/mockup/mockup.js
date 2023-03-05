function on_message_get_projects(message) {
    var reply = `
   {
      "action": "get_projects_callback",
      "response": [
        {
          "id": 
          "097afe69-06ec-4dc4-ac34-aa81979cf899",
          "name": 
          "project1"
        },
        {
          "id": 
          "7df344c7-5c10-4eb2-8ad1-07acf6ac6960",
          "name": 
          "project2"
        },
         {
          "toto": 
          "7df344c7-5c10-4eb2-8ad1-07acf6ac6960",
          "titi": 
          "<svg width='16' height='16'  viewBox='0 0 16 16' xmlns=http://www.w3.org/2000/svg fill='currentColor'>  <path id='mainPath' d='M15 5.5a4.394 4.394 0 0 1-4 4.5 2.955 2.955 0 0 0-.2-1A3.565 3.565 0 0 0 14 5.5a3.507 3.507 0 0 0-7-.3A3.552 3.552 0 0 0 6 5a4.622 4.622 0 0 1 4.5-4A4.481 4.481 0 0 1 15 5.5zM5.5 6a4.5 4.5 0 1 0 0 9.001 4.5 4.5 0 0 0 0-9z'/></svg>"
        }
      ]
    }
`.trim()

    Opal.Object.$controller_test(reply)
    // Opal.Object.$controller_test(reply)
}

function on_message_new_project(message) {
    var reply =
        `
    {
      "action": "new_project_callback",
      "response": {
        "id":
        "097afe69-06ec-4dc4-ac34-aa81979cf899",
        "name":
        "project1" //TODO: Générer un nom de projet aléatoirement.
      }
    }`
            .trim()
    Opal.Object.$controller_test(reply)
}

function on_message_load_project(message) {
    var reply =
        `
          {
      "action": "load_project_callback",
      "response": {
        "id":
        "097afe69-06ec-4dc4-ac34-aa81979cf899",
        "name":
        "project1"
      }
    }
        `
            .trim()

    Opal.Object.$controller_test(reply)
}

function on_message_save_project(message) {
    var reply =
        `
        {
            "action": "save_project_callback",
            "response": {}
        }
        `

            .trim()
    Opal.Object.$controller_test(reply)
}

function on_message_save_project_as(message) {
    var reply =
        `
         {
            "action": "save_project_as_callback",
            "response": {}
        }
        `
            .trim()

    Opal.Object.$controller_test(reply)
}

function on_message_get_modules(message) {
    var reply =
        `
         {
      "action": "get_modules_callback",
      "response":
      [
      {
        "id":
        "424f7df3-9f36-411d-9535-f8323d4f372c",
        "name":
        "Midi in",
        "type_id":
        "midi-in",
        "input_slots":
        [
          {
            "id":
            "378a9a4d-d7d0-4d30-a5f4-0a30e279399e",
            "name":
            "project1",
            "unit":
            "db",
            "value":
            1.0
          }
        ],
        "output_slots":
        [
          {
            "id":
            "916b23e7-7da9-4059-b72c-830b38da0fd5",
            "name":
            "project1",
            "unit":
            "db",
            "value":
            1.0
          }
        ],
        "icon":
            "<svg width='16' height='16'  viewBox='0 0 16 16' xmlns=http://www.w3.org/2000/svg fill='currentColor'>  <path id='mainPath' d='M15 5.5a4.394 4.394 0 0 1-4 4.5 2.955 2.955 0 0 0-.2-1A3.565 3.565 0 0 0 14 5.5a3.507 3.507 0 0 0-7-.3A3.552 3.552 0 0 0 6 5a4.622 4.622 0 0 1 4.5-4A4.481 4.481 0 0 1 15 5.5zM5.5 6a4.5 4.5 0 1 0 0 9.001 4.5 4.5 0 0 0 0-9z'/></svg>"
      }
      ]
    }
        `
            .trim()
    Opal.Object.$controller_test(reply)
}

function on_message_add_module(message) {
    var reply =
        `
        {
            "action": "add_module_callback",
            "response": {
                "id":
                    "097afe69-06ec-4dc4-ac34-aa81979cf899"
            }
        }
     `
            .trim()
    Opal.Object.$controller_test(reply)
}

function on_message_delete_module(message) {
    var reply =
        `
        {
            "action": "delete_module_callback",
            "response": {}
        }
     `
            .trim
    Opal.Object.$controller_test(reply)
}

function on_message_set_module_name(message) {
    var reply =
        `
        {
            "action": "set_module_name_callback",
            "response": {}
        }
     `
            .trim
    Opal.Object.$controller_test(reply)
}

function on_message_set_module_parameter_value(message) {
    var reply =
        `
        {
            "action": "set_module_parameter_value_callback",
            "response": {}
        }
      `
            .trim
    Opal.Object.$controller_test(reply)
}

function on_message_link_modules(message) {
    var reply =
        `
        {
            "action": "link_modules_callback",
            "response": {}
        }
    `
            .trim
    Opal.Object.$controller_test(reply)
}

function on_message_undo(message) {
    var reply =
        `
        {
            "action": "undo_callback",
            "response": {}
        }
       `
            .trim
    Opal.Object.$controller_test(reply)
}

function on_message_redo(message) {
    var reply =
        `
        {
            "action": "redo_callback",
            "response": {}
        }
      `
            .trim()
    Opal.Object.$controller_test(reply)
}