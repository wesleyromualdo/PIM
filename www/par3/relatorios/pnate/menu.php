<?php
$estados = Territorios_Model_Estado::pegarSQLSelect($_POST);
$mundescricao = simec_htmlentities($_REQUEST['muncod']);
if($_REQUEST['esfera'] != 2) {
    $display = 'none';
}
$menuHtml = '
<ul class="nav metismenu" id="side-menu">
    <li>
        <div style="padding: 14px 20px 14px 20px; color: #a7b1c2; font-weight: 600;"><i class="fa fa-search"></i> <span class="nav-label">Filtrar Relatório</span></div>
    </li>
    
    <form action="" method="post" id="relatorioPnate">
        <input type="hidden" name="muncod_hid" value="'.$mundescricao.'">

        <li style="padding: 0 0 15px 20px;">
            <label>Esfera</label>
            '.$simec->radio('esfera', null, $_POST['esfera'] ? $_POST['esfera'] : 1, array('1' => 'Estadual', '2' => 'Municipal')).'
        </li>   
        
        <li style="padding: 0px 18px 35px 6px;">
            <label style="margin-left: 12px;">UF</label>
            '.$simec->select('estuf','', $_POST['estuf'], $estados, ['data-placeholder' => 'Unidade Federativa'], ['input-size' => 12]).'
        </li>        
        
        <li style="padding: 0px 20px 35px 6px; display:'.$display.';" class="municipios">
            <label style="margin-left: 12px;">Município</label>
            '.$simec->select('muncod', null, $mundescricao, array(), ['data-placeholder' => 'Município'], ['input-size' => 12]).'
        </li>
        
        <li class="text-center">
            <button type="submit" name="pesquisar"; class="btn btn-success pesquisar" ><i class="fa fa-search"></i> Pesquisar</button>
        </li>
        <li class="text-center">        
            <script src=\'https://www.google.com/recaptcha/api.js\'></script>   
            <div class="col-sm-2 col-md-2 col-lg-2 ";>        
                <div  style="transform:scale(0.60)"; align="left"; class="g-recaptcha" data-sitekey="'.RECAPTCHA_SITE_KEY.'"></div>  
            </div>        
        </li>
      
        
    </form> 
    
</ul>';

?>
