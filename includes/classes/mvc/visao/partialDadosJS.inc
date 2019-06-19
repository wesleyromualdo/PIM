<script type="text/javascript">
	if (typeof _JS == 'undefined') {
    	var _JS = {};
	}	
<?php
foreach ($this->config['dadosJs'] as $var => $valor) {
    switch(true){
        case $valor === true:
            echo "_JS.{$var} = true;";
            break;
        case $valor === false:
            echo "_JS.{$var} = false;";
            break;
        case $valor === 0:
            echo "_JS.{$var} = 0;";
            break;
        default:
            echo "_JS.{$var} = ". simec_json_encode($valor) . ";";
    }
}
?>
</script>
