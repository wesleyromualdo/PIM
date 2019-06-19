<?php

checkParamInuid();

require APPRAIZ.'includes/cabecalho.inc';

$controleUnidade = new Par3_Controller_InstrumentoUnidade();
$modelUnidade = new Par3_Model_InstrumentoUnidade();

$arrPermissao = $modelUnidade->testaPermissaoUnidade($_REQUEST['inuid']);
if (!$arrPermissao['booVisualizar']) {
    simec_redirecionar('par3.php?modulo=inicio&acao=C', 'error', 'Acesso negado.');
}
?>
<div class="wrapper wrapper-content animated fadeIn">
    <div class="row">
        <div class="ibox float-e-margins">
            <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']; ?>"/>
            <!-- Nome Entidade -->
            <div class="ibox-title">
                <h5 style="font-size: 18px; font-weight: normal;"><?php echo $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']); ?></h5>
            </div>
            <!-- Cabeçalho Unidade + Abas -->
            <div class="ibox-content">
                <?php
                if (!$_REQUEST['requisicao']&&!$_REQUEST['req']) {
                    $controleUnidade->cabecalhoUnidade();
                }

                if (!$_REQUEST['aba']) {
                    $_REQUEST['aba'] = 'documentos';
                }

                switch ($_REQUEST['aba']) {
                    case 'documentos':
                        require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/documentopar.php';
                        break;
                    case 'acompanhamento':
                        require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/contrato/contrato.php';
                        break;
                    case 'outros':
                        require_once APPRAIZ.'par3/modulos/principal/planoTrabalho/acompanhamento/outrosDocumentos.php';
                        break;
                    case 'solicitacaoDesembolso':
                        require_once APPRAIZ.'par3/modulos/principal/planoTrabalho/acompanhamento/solicitacaoDesembolso.inc';
                        break;
                    default:
                        require_once APPRAIZ . 'par3/modulos/principal/planoTrabalho/acompanhamento/documentopar.php';
                        break;
                }
                ?>
                <div class="ibox float-e-margins">
                </div>
            </div>
        </div>
    </div>
</div>

