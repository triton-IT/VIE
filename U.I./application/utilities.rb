# frozen_string_literal: true

def context
  grab(:context).data
end

def debug
  grab(:debug).data[:debug]
end

def log(val)
  puts "=> #{val}" if debug
end
