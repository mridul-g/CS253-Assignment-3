BEGIN {
    FS = ", "
    sum = 0
    count = 0
}
NR>1{
    count++
    sum = sum + $4
}
END{
    print sum/count
}
