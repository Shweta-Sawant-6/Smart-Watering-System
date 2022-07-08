<?php
    $conn = new mysqli("localhost", "vertexte_vertex", "PHrdAQ8luVWM", "vertexte_vertex1");
    $temp=$_GET['temp'];
    $nm = $_GET['soil'];
    $mtr = $_GET['mtr'];
    $sql = "insert into tbl_battery_info(voltage,battery_temp,current) values('$nm','$temp','$mtr')";
    if(mysqli_query($conn,$sql))
    {
        echo "success";
    }
    else
        echo mysqli_error($conn);

?>