<?php
$env_array = getenv();


// else
//     echo $POST;
//Print all environment variable names with values
$array = "";
if ($env_array['REQUEST_METHOD'] == 'GET')
{
    echo "<h3>The list of environment variables with values are :</h3>";
    if (empty($_POST))
        echo "_POST is empty <br />";
    foreach ($env_array as $key=>$value)
    {
        echo "$key => $value <br />";
        if ($key == 'QUERY_STRING'){
            parse_str($value, $array);
        }
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
        echo "<h1 align='center'>Vi kto takie? Ya vas ne zval</h1>";
}
if (array_key_exists('color', $array)){
    echo "<body style=background:"; echo $array['color']; echo ">";
}

?>