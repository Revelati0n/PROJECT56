<?php
date_default_timezone_set("Asia/Bangkok");
echo json_encode(array(
    'DayOfWeek' => date('w'),
    'Hour' => date('H'),
    'Minute' => date('i')
));
?>
