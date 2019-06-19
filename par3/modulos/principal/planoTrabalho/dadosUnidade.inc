<?php

$controleUnidade         = new Par3_Controller_InstrumentoUnidade();
$controleUnidadeEntidade = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelUnidade            = new Par3_Model_InstrumentoUnidade();
$modelAuditoriaUnidade 	 = new Par3_Model_AuditoriaUnidade();
$modelCACS				 = new Par3_Model_CACS();
$controlCAE 	 		 = new Par3_Controller_CAE();
$controleEquipeTecnica   = new Par3_Controller_EquipeTecnica();
checkParamInuid();

if ($_REQUEST['requisicao'] == "inicioPar") {
	$controleUnidadeEntidade->salvarWorkflowDocumento($_REQUEST['inuid']);
    $arrayDados['aducpf'] = $_SESSION['usucpf'];
    $arrayDados['inuid'] = $_REQUEST['inuid'];
    $arrayDados['adutipo'] = 1;
    $arrayDados['adudata'] = date('Y-m-d H:i:s');
    $modelAuditoriaUnidade->popularDadosObjeto( $arrayDados);
    $modelAuditoriaUnidade->inserir();
    $modelAuditoriaUnidade->commit();
    die();
}

if ($_REQUEST['requisicao'] == "resetarSenhaCacs") {
    ob_clean();

    include_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/classes_simec.inc";
    include_once APPRAIZ . "www/par3/_constantes.php";
    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "par3/classes/model/CACS.class.inc";
    include_once APPRAIZ . "par3/classes/model/seguranca/UsuarioSistema.class.inc";
    include_once APPRAIZ . "par3/classes/model/seguranca/Usuario.class.inc";
    include_once APPRAIZ . "par3/classes/model/UsuarioResponsabilidade.class.inc";
    include_once APPRAIZ . "par3/classes/model/seguranca/PerfilUsuario.class.inc";
    include_once APPRAIZ . "seguranca/classes/controller/Usuario.php";

    define("SISID_PAR_02", 23);
    define("PAR_PERFIL_PRESIDENTE", 1503);
    define("PAR_PERFIL_CONSELHEIRO", 1504 );

    $modelCacs = new Par3_Model_CACS();

    $url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid='.$_REQUEST['inuid'].'&menu='.$_REQUEST['menu'];

    if($modelCacs->alterarSenhaUsuarioCacs($_REQUEST['cpf'])){
        simec_redirecionar($url, 'success', 'Senha do usuário alterado com sucesso');
    } else {
        simec_redirecionar($url, 'error', 'Ocorreu um erro ao alterar a senha do usuario');
    }

    exit;
}

$disabled = 'disabled';

$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);

if (!$arrPermissao || !$arrPermissao['booVisualizar']) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}

if ($arrPermissao['booEscrita']) {
    $disabled = '';
}


$itrid  = $controleUnidade->pegarItrid($_REQUEST['inuid']);
$esfera = ($itrid === '2') ? 'Municipal' : 'Estadual';

if ($itrid != '2' && !$_REQUEST['menu']) {
    $_REQUEST['menu'] = 'secretaria';
}

if(!$_REQUEST['requisicao']){
    $testaPreenchimentoPrefeitura     = $controleUnidadeEntidade->verificaPreencimentoPrefeitura($_REQUEST['inuid']);
    $testaPreenchimentoPrefeito       = $controleUnidadeEntidade->verificaPreencimentoPrefeito($_REQUEST['inuid']);
    $testaPreenchimentoSecretaria     = $controleUnidadeEntidade->verificaPreencimentoSecretaria($_REQUEST['inuid']);
    $testaPreenchimentoDirigente      = $controleUnidadeEntidade->verificaPreencimentoDirigente($_REQUEST['inuid']);
    $testaPreenchimentoEquipe         = $controleUnidadeEntidade->verificaPreencimentoEquipe($_REQUEST['inuid']);
    $testaPreenchimentoNutricionistas = $controleUnidadeEntidade->verificaPreencimentoNutricionistas($_REQUEST['inuid']);
    $testaPreenchimentoComite         = $modelCACS->verificaPreenchimentoConselheiros($itrid, $_REQUEST['inuid']);
    $testaPreenchimentoConselho       = $controleUnidadeEntidade->verificaPreencimentoConselho($_REQUEST['inuid']);
    $testaPreenchimentoCAE            = $controlCAE->verificaPreenchimentoCAE($_REQUEST['inuid']);
    $testaPreenchimentoEquipeTecnica  = $controleEquipeTecnica->verificaPreenchimentoEquipeTecnica($_REQUEST['inuid']);


    $iconNaoPreenchido = '<span class="fa fa-times-circle danger" ></span>';
    $iconPreenchido    = '<span class="fa fa-check iconUnidade"'.
                               'style="color:green !important;'.
                                      'background-color: transparent;"></span>';

    $iconPreenchimentoPrefeitura     = $iconNaoPreenchido;
    $iconPreenchimentoPrefeito       = $iconNaoPreenchido;
    $iconPreenchimentoSecretaria     = $iconNaoPreenchido;
    $iconPreenchimentoDirigente      = $iconNaoPreenchido;
    $iconPreenchimentoEquipe         = $iconNaoPreenchido;
    $iconPreenchimentoPrefeitura     = $iconNaoPreenchido;
    $iconPreenchimentoComite         = $iconNaoPreenchido;
    $iconPreenchimentoNutricionistas = $iconNaoPreenchido;
    $iconPreenchimentoTecnicos       = $iconNaoPreenchido;
    $iconPreenchimentoConselho       = $iconNaoPreenchido;
    $iconPreenchimentoCAE            = $iconNaoPreenchido;

    if ($testaPreenchimentoPrefeitura === 100)
        $iconPreenchimentoPrefeitura = $iconPreenchido;
    if ($testaPreenchimentoPrefeito === 100)
        $iconPreenchimentoPrefeito = $iconPreenchido;
    if ($testaPreenchimentoSecretaria === 100)
        $iconPreenchimentoSecretaria = $iconPreenchido;
    if ($testaPreenchimentoDirigente === 100)
        $iconPreenchimentoDirigente = $iconPreenchido;
    if ($testaPreenchimentoEquipe === 100)
        $iconPreenchimentoEquipe = $iconPreenchido;
    if ($testaPreenchimentoComite === 100)
        $iconPreenchimentoComite = $iconPreenchido;
    if ($testaPreenchimentoNutricionistas === 100)
        $iconPreenchimentoNutricionistas = $iconPreenchido;
    if ($testaPreenchimentoEquipeTecnica === 100)
        $iconPreenchimentoTecnicos = $iconPreenchido;
    if ($testaPreenchimentoConselho === 100){
    	$iconPreenchimentoConselho = $iconPreenchido;
    }

    if ($testaPreenchimentoCAE === 100)
    	$iconPreenchimentoCAE = $iconPreenchido;

    $modelAuditoriaUnidade = new Par3_Model_AuditoriaUnidade();
    $verificaInicioPar = $modelAuditoriaUnidade->verificarInicioPar($_REQUEST['inuid']);

    $perfils = pegaPerfils($_SESSION['usucpf']);

    $arrSemResponsabilidade = array(
    //     Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,
    //     Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,
        Par3_Model_UsuarioResponsabilidade::EQUIPE_MUNICIPAL,
        Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL,
        Par3_Model_UsuarioResponsabilidade::PREFEITO,
        Par3_Model_UsuarioResponsabilidade::EQUIPE_ESTADUAL,
        Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL,
        Par3_Model_UsuarioResponsabilidade::SECRETARIO_ESTADUAL,
    );

    $verificaInicioPar = checaPerfilValido($arrSemResponsabilidade, $perfils) == 'S' ? $verificaInicioPar : false;
}

$mostrar = false;//Solicitado para não mostrar esta opção no menu
require APPRAIZ.'includes/cabecalho.inc';
?>
<style>
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

@media (max-width: 1450px) {
    .ibox-content-round-gray{
        margin-top: 0.5%
    }
}

@media (max-width: 1200px) {
    .i1450 {
        display: inline;
    }
    .custom-col-10{
        padding: 5px 0px 0px 3.8%;
    }
    .marcador{
        position:absolute;
        border-radius: 100% 0% 0% 100%;
        -moz-border-radius: 100% 0% 0% 100%;
        -webkit-border-radius: 100% 0% 0% 100%;
        border: 0px;
        width:50px;
        height:10px;
        margin-top:21px;
        margin-left:-1%;
    }
}


</style>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
<!--                <div class="ibox-tools">-->
<!--                    <a class="tourn-btn" data-toggle="tooltip" title="" data-placement="bottom" href="#" data-original-title="Ajuda?">-->
<!--                        <i class="fa fa-question-circle"></i>-->
<!--                    </a><div class="tooltip fade bottom" role="tooltip" id="tooltip94904" style="top: 18px; left: -15.5px; display: block;"><div class="tooltip-arrow"></div><div class="tooltip-inner">Ajuda?</div></div>-->
<!--                </div>-->
            </div>
            <div class="ibox-content">
                <?php if(!$_REQUEST['requisicao']) $controleUnidade->cabecalhoUnidade(); ?>
                <div class="ibox-content-round-gray ibox-content">
                    <div class="row">
                        <div class="col-md-3 colunaMenu noprint">
                        	<div class="space-25"></div>
                        	<div id="menuDirigentes">
	                        	<h5>DIRIGENTES</h5>
		                        <?php if ($itrid === '2') { ?>
		                            <div class="menuUnidade <?php echo ( ($_REQUEST['menu'] == '' || $_REQUEST['menu'] === 'prefeitura') ? 'menuSelecionado' : '') ?>" tipo="prefeitura">
		                                <?php echo $iconPreenchimentoPrefeitura; ?>
		                                Prefeitura
		                            </div>
		                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'prefeito' ? 'menuSelecionado' : '') ?>"
		                                 tipo="prefeito">
		                                <?php echo $iconPreenchimentoPrefeito; ?>
		                                Prefeito
		                            </div>
		                        <?php }; ?>
	                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'secretaria' ? 'menuSelecionado' : '') ?>" tipo="secretaria">
	                                <?php echo $iconPreenchimentoSecretaria; ?>
	                                Secretaria <?php echo $esfera; ?> de Educação
	                            </div>
                                <?php  if ($itrid === '2'){
                                    if(possuiPerfil(array(Par3_Model_UsuarioResponsabilidade::ADMINISTRADOR,Par3_Model_UsuarioResponsabilidade::SUPER_USUARIO,Par3_Model_UsuarioResponsabilidade::PREFEITO))){
                                    ?>
                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'dirigente' ? 'menuSelecionado' : '') ?>" tipo="dirigente">
                                    <?php echo $iconPreenchimentoDirigente.' Dirigente '.$esfera.' de Educação'; ?>
                                </div>
                                <?php }} else {?>
                                    <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'secretario' ? 'menuSelecionado' : '') ?>" tipo="secretario">
                                        <?php echo $iconPreenchimentoDirigente.' Secretário(a) '.$esfera.' de Educação'; ?>
                                    </div>
                                <?php }?>
							</div>

                            <div class="space-25"></div>
                            <div id="menuEquipeTecnica">
	                        	<h5>EQUIPE</h5>
                                <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'tecnico' ? 'menuSelecionado' : '') ?>" tipo="tecnico">
                                    <?php echo $iconPreenchimentoTecnicos; ?>
                                    Equipe Técnica
                                </div>
	                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'equipe' ? 'menuSelecionado' : '') ?>" tipo="equipe">
	                                <?php echo $iconPreenchimentoEquipe; ?>
	                                Equipe Local
	                            </div>
	                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'nutricionista' ? 'menuSelecionado' : '') ?>" tipo="nutricionista">
	                                <?php echo $iconPreenchimentoNutricionistas; ?>
	                                Equipe Nutricionistas
	                            </div>
	                        </div>
                            <div class="space-25"></div>
                            <div id="menuConselhos">
	                        	<h5>CONSELHOS</h5>
	                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'comite' ? 'menuSelecionado' : '') ?>" tipo="comite">
	                                <?php echo $iconPreenchimentoComite; ?>
	                                Conselho de Acompanhamento e Controle Social
	                            </div>
	                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'conselho' ? 'menuSelecionado' : '') ?>" tipo="conselho">
	                                <?php echo $iconPreenchimentoConselho; ?>
	                                Conselho <?php echo $esfera; ?> de Educação
	                            </div>
                                <?php if($mostrar):?>
	                            <div class="menuUnidade <?php echo ( $_REQUEST['menu'] === 'cae' ? 'menuSelecionado' : '') ?>" tipo="cae">
	                                <?php echo $iconPreenchimentoCAE; ?>
	                                Conselho de Alimentação Escolar
	                            </div>
                                <?php endif;?>
	                       	</div>
                        </div>
                        <div class="col-md-9 colunaForm" id="print_area">
                            <?php $controleUnidade->desenharTela($_REQUEST['menu']); ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div id="modal-form" class="modal fade" aria-hidden="true">
	<div class="modal-dialog" style="width: 850px">
		<div id="html_modal-form" style="width: 850px">
		</div>
	</div>
</div>
<?php
	if ($itrid == 2) {
	    $nivel = "município";
	} else {
	    $nivel = "estado";
	}
	$nome = explode(" ",$_SESSION['usunome'])
?>
<script src="js/tour/dadosUnidade.js"></script>
<script>
$(document).ready(function(){
    <?php if ($verificaInicioPar) : ?>
    swal({
        title: "<span style='font-size: 27px'><?php echo ucfirst(strtolower($nome[0]))?>, seja bem-vindo(a) ao Plano de Ações Articuladas</span>",
        text: "Você deseja iniciar o planejamento do seu <?php echo $nivel?>? ",
        showCancelButton: true,
        confirmButtonColor: "#DD6B55", confirmButtonText: "Sim, tenho certeza!",
        closeOnConfirm: "on",
        cancelButtonText: "Cancelar",
        html: true
    }, function (isConfirm) {
        if (isConfirm) {
            var param = new Array();
            param.push({name: 'requisicao', value: 'inicioPar'});
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: param,
                success: function() {
                    $('.tourn-btn').trigger('click');
                }
            });
        } else {
            window.location.href = 'par3.php?modulo=inicio&acao=C&redirect=false;';
        }
    });
    <?php endif; ?>

    $('.menuUnidade').click(function(){
        var menuAtual = '<?php echo $_REQUEST['menu']; ?>';
        var menu      = $(this).attr('tipo');
        var url       = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A'
                        +'&inuid=<?php echo $_REQUEST['inuid']?>&menu='+menu;

        if (menu != menuAtual) {
            $(location).attr('href',url);
        }
    });

    $('#entcpf,#entcnpj').change(function()
    {
        var form = $(this).parent().parent().parent().parent().find('#form').val();
        var id   = $(this).attr('id');

        if (!form) {
            if ($(this).val() != $('[name="'+id+'_old"]').val() && $('[name="'+id+'_old"]').val() != '') {
                $('input[name!="'+id+'"][type=text]').val('');
            }
        } else {
            if (($(this).val() != $('#'+form).find('[name="'+id+'_old"]').val()) && ($('#'+form).find('[name="'+id+'_old"]').val() != '')) {
                $('#'+form).find('input[name!="'+id+'"][type=text]').val('');
            }
        }
    });
});

function inativaEntidade(entid) {
    if (!confirm("Deseja realmente escluir o conselheiro?")) {
        return false;
    }

    var url   = window.location.href;
    var param = '&req=excluirEntidade&entid='+entid;
    window.location.href = url+param;
}

function editarEntidade(entid) {
    var url   = window.location.href.toString().replace(/(&req=[a-zA-Z]+)+(&entid=\d+)/g, '');
    var param = '&req=editarEntidade&entid='+entid;
    location.href = url+param;
}
</script>