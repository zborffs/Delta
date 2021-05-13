using Flux, Images
using StatsBase: sample, shuffle

const PATH = joinpath(@__DIR__, "../../res/data/output_train/")
const BP = joinpath.(string(PATH, "bp/"), readdir(string(PATH, "bp/")))
const WP = joinpath.(string(PATH, "wp/"), readdir(string(PATH, "wp/")))
const BR = joinpath.(string(PATH, "br/"), readdir(string(PATH, "br/")))
const WR = joinpath.(string(PATH, "wr/"), readdir(string(PATH, "wr/")))
const BN = joinpath.(string(PATH, "bn/"), readdir(string(PATH, "bn/")))
const WN = joinpath.(string(PATH, "wn/"), readdir(string(PATH, "wn/")))
const BB = joinpath.(string(PATH, "bb/"), readdir(string(PATH, "bb/")))
const WB = joinpath.(string(PATH, "wb/"), readdir(string(PATH, "wb/")))
const BQ = joinpath.(string(PATH, "bq/"), readdir(string(PATH, "bq/")))
const WQ = joinpath.(string(PATH, "wq/"), readdir(string(PATH, "wq/")))
const BK = joinpath.(string(PATH, "bq/"), readdir(string(PATH, "bq/")))
const WK = joinpath.(string(PATH, "wq/"), readdir(string(PATH, "wq/")))
const EMPTY = joinpath.(string(PATH, "empty/"), readdir(string(PATH, "empty/")))


function load_batch(n = 130, nsize = (227,227); path = PATH)
  imgs_paths = shuffle(vcat(
    sample(BP, Int(n/13)),
    sample(WP, Int(n/13)),
    sample(BR, Int(n/13)),
    sample(WR, Int(n/13)),
    sample(BN, Int(n/13)),
    sample(WN, Int(n/13)),
    sample(BB, Int(n/13)),
    sample(WB, Int(n/13)),
    sample(BQ, Int(n/13)),
    sample(WQ, Int(n/13)),
    sample(BK, Int(n/13)),
    sample(WK, Int(n/13)),
    sample(EMPTY, Int(n/13))
  ))
  labels = map(x -> occursin("bp", x) ? 0 :
                    occursin("wp", x) ? 1 :
                    occursin("br", x) ? 2 :
                    occursin("wr", x) ? 3 :
                    occursin("bn", x) ? 4 :
                    occursin("wn", x) ? 5 :
                    occursin("bb", x) ? 6 :
                    occursin("wb", x) ? 7 :
                    occursin("bq", x) ? 8 :
                    occursin("wq", x) ? 9 :
                    occursin("bk", x) ? 10 :
                    occursin("wk", x) ? 11 : 12, imgs_paths)
  labels = Flux.onehotbatch(labels, 0:12)
  imgs = Images.load.(imgs_paths)
  # imgs = map(img -> Images.imresize(img, nsize...), imgs)
  imgs = map(img -> permutedims(channelview(img), (3,2,1)), imgs)
  imgs = cat(imgs..., dims = 4)
  Float32.(imgs), labels
end
