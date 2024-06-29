BEGIN {
    FS = ", "
}
NR>1{
    sum[$3]+=$4
    count[$3]++
}
END{
    for(city in sum) 
        print "City: "city", Salary: "sum[city]/count[city]
}