BEGIN {
    FS = ","
}
NR>1{
    if($4 > avg)
    {
        print $1, $2, $3, $4
    }
}
