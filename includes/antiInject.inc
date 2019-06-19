<?php
/**
 * Created by PhpStorm.
 * User: lotavino
 * Date: 06/02/19
 * Time: 10:19
 */
if(anti_injection($_SERVER['REQUEST_URI'])){
    ?>
    <script>
        window.location.href="/logout.php";</script><?
    exit ();
}