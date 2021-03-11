## Contains analysis tools for system and controller
mutable struct StateSpace
    A::Array{Float64,2}
    B::Array{Float64,2}
    C::Array{Float64,2}
    D::Array{Float64,2}

    StateSpace(A, B, C, D) = new(A, B, C, D)
end

mutable struct TransferFunction

end

## Analysis
function isstable()
end

function isobservable()
end

function observability()
end

function iscontrollable()
end

function controllability()
end

function damp()
end

function dcgain()
end

function delaymargin()
end

function margin()
end

function pole()
end

function pid_stability_region()
end

function tzero()
end

function zpkdata()
end

## Synthesis
"""
calculate the optimal Kalman gain
"""
function kalman()
end

function loopshapingPI()
end

function lqr()
end

function pid()
end

function place()
end

## Time and Frequency Response
function bode()
end

function evalfr()
end

function freqresp()
end

function impulse()
end

function nyquist()
end

function step()
end

## Plotting Recipes
@recipe function bodeplot()
end

@recipe function impulseplot()
end

@recipe function leadlinkcurve()
end

@recipe function marginplot()
end

@recipe function nicholsplot()
end

@recipe function nyquistplot()
end

@recipe function pidplots()
end

@recipe function pzmap()
end

@recipe function rlocus()
end

@recipe function stepplot()
end
