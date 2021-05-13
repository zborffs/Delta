using Statistics
using Flux: onehotbatch, onecold, crossentropy, throttle
using Base.Iterators: repeated
#using CuArrays if you want to use GPU

imgs = Flux.Data.MNIST.images()
labels = Flux.Data.MNIST.labels();

X = hcat(float.(reshape.(imgs, :))...) #stack all the images
Y = onehotbatch(labels, 0:9) # just a common way to encode categorical variables

m = Chain(
  Dense(28^2, 32, relu),
  Dense(32, 10),
  softmax)

loss(x, y) = crossentropy(m(x), y)
opt = ADAM(); # popular stochastic gradient descent variant

accuracy(x, y) = mean(onecold(m(x)) .== onecold(y)) # cute way to find average of correct guesses

dataset = repeated((X,Y),200) # repeat the data set 200 times, as opposed to @epochs 200 ...
evalcb = () -> @show(loss(X, Y)) # callback to show loss


Flux.train!(loss, params(m), dataset, opt, cb = throttle(evalcb, 10)); #took me ~5 minutes to train on CPU

# Same preprocessing
test_X = hcat(float.(reshape.(Flux.Data.MNIST.images(:test), :))...)
test_Y = onehotbatch(Flux.Data.MNIST.labels(:test), 0:9);

m(test_X[:,5287]) # Note the 7th index ( corresponding to the digit 6 ) is nearly 1

#decode
onecold(m(test_X[:,5287])) - 1 #minus 1 since we start from 0, but indexing in Julia starts at 1

# Training set accuracy
accuracy(X, Y)


#######
using Flux, Metalhead
using Flux: @epochs
resnet = ResNet().layers

model = Chain(
  resnet[1:end-2],
  x -> reshape(x, 512),
  Dense(512, 13)
)

include("dataloader.jl")

dataset = [gpu.(load_batch(13*5)) for i in 1:10]

model = model |> gpu
opt = ADAM()

loss(x,y) = Flux.Losses.logitcrossentropy(model(x), y)

ps = Flux.params(model[2:end])

@epochs 2 Flux.train!(loss, ps, dataset, opt)

imgs, labels = gpu.(load_batch(13))
display(model(imgs))

labels

using Statistics
using Flux: onehotbatch, onecold, crossentropy, throttle
using Base.Iterators: repeated

accuracy(x, y) = mean(onecold(model(x)) .== onecold(y)) # cute way to find average of correct guesses

X = imgs
Y = labels

X1, Y1 = dataset[1][1], dataset[1][2]
X2, Y2 = dataset[2][1], dataset[2][2]

accuracy(X, Y)
accuracy(X1, Y1)
accuracy(X2, Y2)
