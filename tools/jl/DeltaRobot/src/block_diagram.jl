## functions for drawing block diagrams
using Luxor

mutable struct BlockDiagram
    id::Integer
    name::AbstractString
    inputs::Array{AstractString, 1}
    outputs::Array{AstractString, 1}
end

function feedback(b1::BlockDiagram, b2::BlockDiagram)
end

function series(b1::BlockDiagram, b2::BlockDiagram)
end

function parallel(b1::BlockDiagram, b2::BlockDiagram)
end

function draw(b::BlockDiagram)
end
