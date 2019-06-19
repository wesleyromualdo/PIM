<?php
global $simec, $arrEntidade;

$iconNaoPreenchido = '<span class="fa fa-times-circle danger" ></span>';
$iconPreenchido    = '<span class="fa fa-check iconUnidade"'.
    'style="color:green !important;'.
    'background-color: transparent;"></span>';

$pastaDadosUnidade = 'demandas/modulos/visao/';
$arrEntidade = array();

?>
<style>
.custom_menu > li.active {
    border-left: none;
    background: none; 
}
    
.iconUnidade{
    margin-top: 0px !important;
    margin-left: 0px !important;
    font-size: 15px !important;
    background-color:black;
    border-radius: 20px 20px 20px 20px;
    -moz-border-radius: 20px 20px 20px 20px;
    -webkit-border-radius: 20px 20px 20px 20px;
    color: yellow !important;
}

.menuUnidade{
    border-bottom: 1px solid #e3e3e3;
    padding: 5px;
    cursor:pointer;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
}

.menuUnidade:hover{
    font-weight: bold;
}

.menuSelecionado{
    font-weight: bold;
    cursor: default !important;
}
</style>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-12">
                <h2 class="left"><?php echo $this->enjdsc; ?></h2>
            </div>
        </div>
    </div>
    <div class="ibox-title">
        <?php echo montaCabecalhoDemanda($_REQUEST['ddtid']);?>
    </div>
    <div class="ibox-content">
        <div class="ibox-content-round-gray ibox-content">
            <div class="row">
                <div class="col-md-3 colunaMenu noprint">
                	<div class="space-25"></div>
                	<div id="menuDirigentes">
                    	<h5>INFORMAÇÕES</h5>
                            <div class="menuUnidade <?php echo ( ($_REQUEST['enfid'] == '' || $_REQUEST['enfid'] === ($this->funcao_entidade[0]['enfidpai']) ) ? 'menuSelecionado' : '') ?>" tipo="J" enfid="<?php echo $this->funcao_entidade[0]['enfidpai'];?>">
                                <?php echo $iconPreenchido; ?>
                                <?php echo $this->funcao_entidade[0]['entidade_pai']?>
                                <?php $arrEntidade[$this->funcao_entidade[0]['enfidpai']] = $this->funcao_entidade[0]['entidade_pai']; ?>
                            </div><br>
                        <h5>REPRESENTANTES</h5>
<?php                   foreach ($this->funcao_entidade as $v) {
                            $arrEntidade[$v['enfidfilho']] = $v['entidade_filho'];?>
							<div class="menuUnidade <?php echo ( $_REQUEST['enfid'] === ($v['enfidfilho']) ? 'menuSelecionado' : '') ?>" tipo="<?php echo $v['enftipo'];?>" enfid="<?php echo $v['enfidfilho']; ?>">
                                <?php echo $iconPreenchido; ?>
                                <?php echo $v['entidade_filho']; ?>
                            </div>    
<?php }?>
					</div>

                </div>
                <div class="col-md-9 colunaForm" id="print_area">
                    <?php 
                    switch ($_REQUEST['tipo']) {
                        case 'F':
                            $arrEntidade['entidade'] = $arrEntidade;
                            $this->carregarDados($arrEntidade);
                            echo $this->incluirVisaoAcessoria('formListaPessoaFisica');
                            break;
                        case 'J':
                            $arrEntidade['entidade'] = $arrEntidade;
                            $this->carregarDados($arrEntidade);
                            echo $this->incluirVisaoAcessoria('formListaPessoaJuridica');
                            break;
                        default:
                            $_REQUEST['tipo'] = 'J';
                            $_REQUEST['enfid'] = '1';
                            $arrEntidade['entidade'] = $arrEntidade;
                            $this->carregarDados($arrEntidade);
                            echo $this->incluirVisaoAcessoria('formListaPessoaJuridica');
                            
                            //include APPRAIZ . $pastaDadosUnidade . 'pessoaJuridica.php';
                            break;
                    }
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">



$('.menuUnidade').click(function(){
    var menuAtual = '<?php echo $_REQUEST['enfid']; ?>';
    var tipo      = $(this).attr('tipo');
    var enfid      = $(this).attr('enfid');
    var url       = 'demandas.php?modulo=demandantes/verDadosDemandante&acao=A&ddtid=<?php echo $_REQUEST['ddtid'];?>&enfid='+enfid+'&tipo='+tipo; 
    
    if (enfid != menuAtual) {
        $(location).attr('href',url);
    }
});
</script>