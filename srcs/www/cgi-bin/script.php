<?php
// echo "HTTP/1.1 200 OK\r\n";
// header("HTTP/1.1. 200 OK");
// echo "Content-type: text/html\r\n\r\n";
// echo "Transfer-Encoding: chunked\r\n\r\n";
// echo "Location: http://example.com\r\n\r\n";
// phpinfo();
// if (headers_sent())
//     echo "header sent";
// else
//     echo "header not sent";

// header("\r\n\r\n");
//Print all environment variable names with values
$array;
if ($_SERVER['REQUEST_METHOD'] == 'GET')
{
    parse_str($_SERVER['QUERY_STRING'], $array);
    echo "<h3>The list of environment variables with values are :</h3>";
    foreach ($_SERVER as $key=>$value)
    {
        if (is_array($value))
            foreach ($value as $_key=>$_value)
                echo "$key [$_key] => $_value <br />";
        else
            echo "$key => $value <br />";
        
    }
}
else
{
    while( $line = fgets(STDIN) ) {
        parse_str($line, $array);
    }   
}
if (array_key_exists('user', $array)){
    if ($array['user'] == 'echiles')
        echo "<h1 align='center'>zdarova Leha</h1>";
    else if ($array['user'] == 'abirthda')
        echo "<h1 align='center'>Boba da sun</h1>";
    else if ($array['user'] == 'agigi')
        echo "<h1 align='center'>Welcome Dima</h1>";
    else
        echo "<h1 align='center'>Vi kto takie? Ya vas ne zval</h1></br>
                <h3 align='center'> Idite nahooy</h3>";
}
if (array_key_exists('color', $array)){
    echo "<body style=background:"; echo $array['color']; echo ">";
}

?>