def get_k(output)
  return -1 if output.chomp == ""
  return output.split("\n").select{ |line| ! line.start_with?("#")}.size
end

def get_debug(output)
  return {} if output.chomp == ""
  debug_outstring = output.split("\n").select{ |line| line.start_with?("#debug:")}
  if(debug_outstring.size() > 0)
    return JSON.parse(debug_outstring[0].slice(8, debug_outstring[0].size))
  end
  return {}      
end

def get_metrics(k, kcorrect)
  solved = k != -1
  if(kcorrect != -1) # we have no correct result
    return {
      "absolut" => k,
      "no_correct" => true,
      "solved" => solved,
      "k_correct" => kcorrect
    }
  else
    if(kcorrect != 0)
      qual = kcorrect.to_f/k.to_f
      qual_inv = k.to_f / kcorrect.to_f
    else # it has to be k == 0, because they should solve right
      qual = 1
    end
    
    return {
      "absolut" => k,
      "no_correct" => false,
      "quality" => qual,
      "quality_inv" => qual_inv,
      "distance" => kcorrect - k,
      "solved" => solved,
      "k_correct" => kcorrect
    
    }
  end
  
end