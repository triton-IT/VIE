# frozen_string_literal: true

# we init the controller_listener
Atome.controller_listener

def response_listener(hashed_msg)
  js_action = hashed_msg.JS[:action]
  js_body = hashed_msg.JS[:body]
  send(js_action, js_body)
end

def load_modules(_module_id)
  Atome.controller_sender({ action: :get_modules })
end

def rename_project
  Atome.controller_sender({ action: :new_project })
end

def delete_project
  log "delete : #{grab(:current_project).data}"
end

def duplicate_project
  log "duplicate : #{grab(:current_project).data}"
end
