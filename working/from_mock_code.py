from math import ceil, exp, log as ln # in C++, log from #include <math.h> is ln as well

# euler = 2.718281828459045

def midiToCV(midi): return midi * 1/12

def vAtTimeEq(t, tau, vInitial, vGoal):
  '''get voltage, vAtTime, after
  t (seconds) of RC cicuit given...
  tau (time constant = C * R)
  vInitial (voltage of Capacitor at t=0)
  vGoal (voltage is applied to non-C side of R)'''
  return vGoal - (vGoal - vInitial) * exp(- t/tau)

def tauEq(t, vAtTime, vInitial, vGoal):
  '''get tau (time constant = C * R) of RC circuit given:
  t (seconds elapsed since vGoal was applied)
  vAtTime (voltage of Capacitor after t seconds)
  vInitial (voltage of Capacitor at t=0)
  vGoal (voltage is applied to non-C side of R)'''
  return t/( ln( (vGoal-vInitial)/(vGoal-vAtTime) ) )





class PAR:
  def writeOut():
    pass
  def analogRead():
    pass



# def trySemitoneDivs(div_per_semi):
#   semitone = 1/12
#   max_v = semitone/div_per_semi * 4095 # Not 4096, I think, since with 4095 I got a quarter-tone 
#   keys = 12 * voltage_span # = voltage_span/semitone
#   gain = max_v/4.095

# voltage span is number of notes available - 1
def VoltageSpanFromDivPerSemi(div_per_semi): return 1365/(4*div_per_semi) # == 4095 * semitone/div_per_semi and...
# not 4096, I think, since with 4095 I got a quarter-tone 
# not 1024/(3*self.div_per_semi) # == 4096 * semitone/div_per_semi

def noteSpanFromVoltageSpan(voltage_span): return 12 * voltage_span # == voltage_span/semitone == voltage_span/(1/12) 
def gainFromVoltageSpan(voltage_span): return voltage_span/4.095
def gainFromNoteSpan(note_span): return (note_span * 50)/2457  # (x/12)/4.095 simplifies to (x * 50)/2457

class PitchPAR(PAR):
  
  def __init__(self, div_per_semi_32_lte_56=42):
    self.calibCVScaling(div_per_semi_32_lte_56)

  def calibCVScaling(self, div_per_semi_32_lte_56=42):
    '''
    user selects division per semitone (32...56, both inclusive), and gets display of 
    what the output voltage range should be along with info on the note range that'll 
    be available. 

    Preferred settings: [50, 42, 39, 35, 32] 
      semitone/50, 6.825+min V, 82.9  notes: 24 to 105.9, gain=1.666...
      semitone/42, 8.125+min V, 98.5  notes: 12 to 109.5, gain=1.984126...
      semitone/39, 8.75+min V,  106.0 notes: 12 to 117,   gain=2.1367521...
      semitone/35, 9.75+min V,  118.0 notes: 0 to 117,    gain=2.380952...
      semitone/32, 10.6640625+min V, 128.96875 notes: 0 to 127.96875, gain=2.6041666...
      
    TODO: we need to work out the langs rounding errors
    UPDATE: now taking div per semitone which for most selections, will not have 
    rounding errors
    '''
    
    # TODO: remove next line and uncomment line after it
    self.div_per_semi = div_per_semi_32_lte_56
    # self.div_per_semi = min(max(div_per_semi_32_lte_56, 32), 56) # clip max and min to what's possible on HW

    # self.voltage_span = 1024/(3*self.div_per_semi) # == 4096 * semitone/div_per_semi a
    self.voltage_span = 1365/(4*self.div_per_semi) # == 4095 * semitone/div_per_semi and...
    # not 4096, I think, since with 4095 I got a quarter-tone 

    self.note_span = 12 * self.voltage_span # voltage span is number of notes available - 1
    # notes_available = int(self.note_span + 1)
    notes_available = self.note_span + 1
    
    self.setMinNote() # NOTE: if user already set base midi in note, it will be reset to default here!
    return f'resolution: semitone/{self.div_per_semi}, {self.voltage_span}+min V, {notes_available} notes: {self.note_min} to {self.note_max}'

  def setMinNote(self, min_note_sint=None):
    # NOTE: we won't prevent setting to minimums which are high that they make otherwise reachable
    #       high note become unreachable. No judgements here, but be careful what you wish for.
    
    missing_notes = max(127 - self.note_span, 0) # max because FULL_MIDI would make missing_notes be -0.96875

    if min_note_sint is None:
      self.note_min = ceil(int( missing_notes / 12 )/2) * 12 # set to default..
    else:
      # NOTE: in C++, min_note_sint can't be negative either, but in python we'll clip it to zero
      # NOTE: we won't, however, prevent minimums that limit range, as noted before, even if it clips it all out!
      self.note_min = min(max(min_note_sint, 0), 127)
      
    self.note_max = self.note_span + self.note_min


  def writeOut(self, midi_pitch):
    midi_pitch = self._midiPitchEnforceBounds(midi_pitch)
    # TODO: convert to DACmV
  
  def _midiPitchEnforceBounds(self, midi_pitch):
    # This will not fail any midi_pitch : instead it will return same note in the nearest octave
    
    if midi_pitch < self.note_min:
      # from "Smallest number greater than or equal to N divisible by K" (N=note_min, K=12)
      # https://www.geeksforgeeks.org/smallest-number-greater-than-or-equal-to-n-divisible-by-k/
      rem = (self.note_min + 12) % 12; 
      if (rem == 0): return self.note_min
      else: return (self.note_min + 12 - rem)

    elif midi_pitch > self.note_max:
      # from "Largest number smaller than or equal to N divisible by K" (N=note_max, K=12)
      # https://www.geeksforgeeks.org/largest-number-smaller-than-or-equal-to-n-divisible-by-k/
      rem = self.note_max % 12
      if(rem == 0): return self.note_max
      else: return self.note_max - rem

    else:
      return midi_pitch

  def midiGoalToDACmV(self, midi_flt):
    return midi_flt/127 * 4095
  
##### for test

max_hz = 1000 # this is 1ms, as was observed to be the max rate on real HW (for about 6 tau or something)
iabc_multiplier_for_cycle_per_ms  = max_hz/1814.37 # 0.5511554975005099

class simPAR():
  def __init__(self, nF=10, vNow_init=0):
    self.C = nF * 1e-9
    self.vNow = vNow_init

from datetime import datetime

class microsDualTimer():
  # arduino docs say micros() will wrap to 0 "after approximately 70 minutes" which is 
  # 4.2e+9 = 4200000000 us. I'm thinking this means it is 32bit unsigned which wraps 
  # to 0 above 2**32 - 1 (AT 2**32), which is 71.5827882666 min (where 6 repeats)
  def __init__(self):
    self.timer1_start = datetime.datetime.now() # we'll generally use this timer for total time
    self.timer2_start = datetime.datetime.now() # we'll use this for "time since reset"

  def timer1Elapsed(self, reset=False):
    now = datetime.datetime.now()
    elapsed = (now - self.timer1_start).total_seconds()*1000000 
    if reset: self.timer1_start = now
    return elapsed

  def timer2Elapsed(self, reset=False):
    now = datetime.datetime.now()
    elapsed = (now - self.timer2_start).total_seconds()*1000000
    if reset: self.timer2_start = now
    return elapsed

