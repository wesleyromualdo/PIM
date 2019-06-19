<?php
/**
 * Tela de dados do conselho
 *
 * @category visao
 * @package  A1
 * @author   Fellipe Esteves <fellipesantos@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
$renderConselho     = new Par3_Controller_Entidade();
$controleUnidade    = new Par3_Controller_InstrumentoUnidade();
$modelUnidade       = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
$controllerInstrumentoUnidadeEntidade   = new Par3_Controller_InstrumentoUnidadeEntidade();
$modelInstrumentoUnidadeEntidade        = new Par3_Model_InstrumentoUnidadeEntidade();

$inuid = $_REQUEST['inuid'];
$itrid = $controleUnidade->pegarItrid($inuid);

if($modelUnidade->inusemconselhoeducacao == 't') $disabled = 'disabled="disabled"';

if ($itrid === '2') {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_MUNICIPAL;
} else {
    $tenid = Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_ESTADUAL;
}

switch ($_REQUEST['req']) {
    case 'salvarConselho':
        $controllerInstrumentoUnidadeEntidade->salvarInformacoesConselho($_REQUEST);
        break;
    case 'removerConselheiro':
        $controllerInstrumentoUnidadeEntidade->inativarConselheiro($_REQUEST);
        break;
    case 'sem_conselho':
        $modelUnidade->inusemconselhoeducacao = $modelUnidade->inusemconselhoeducacao != 't' ? 'TRUE' : 'FALSE';
        $modelUnidade->salvar();
        $modelUnidade->commit();

        ob_clean();
        echo $modelUnidade->inusemconselhoeducacao;
        die();
        break;
    default:
        $arrConselheiros = $modelInstrumentoUnidadeEntidade->carregarConselheiros($inuid, $tenid);

        $arrParamHistorico = array();
        $arrParamHistorico['inuid'] = $inuid;
        $arrParamHistorico['tenid'] = $tenid;
        $arrParamHistorico['booCPF'] = false;
        break;
}
?>
<div class="ibox">
    <div class="ibox-title">
        <div class="row">
            <div class="col-md-6">
                <h3>Conselheiros <?php echo $esfera; ?> de Educação</h3>
            </div>
            <div class="col-md-6">
                <button class="btn btn-success novo pull-right" data-toggle="modal" data-target="#modal" id="btnInserir" <?php echo ($disabled != '') ? 'style="display:none;"' : ''; ?>>
                    <i class="fa fa-plus-square-o"></i>
                    Inserir Conselheiros
                </button>
            </div>
        </div>
    </div>
    <?php if ($itrid === '2') {?>
	<div class="ibox-content">
        <?php $attr = Par3_Model_UsuarioResponsabilidade::permissaoEscrita($disabled, $disabledForm); ?>
        <input <?php echo $attr; ?> type="checkbox" class="sem_conselho" <?php echo ($modelUnidade->inusemconselhoeducacao == 't') ? 'checked="checked"' : '' ;?> />Este município não possui Conselho Municipal de Educação, tendo as respectivas funções efetuadas pelo Conselho Estadual de Educação.
	</div>
	<?php }?>
    <?php if (count($arrConselheiros) > 0 && is_array($arrConselheiros)): ?>
        <div class="ibox-content" id="lista" <?php echo ($modelUnidade->inusemconselhoeducacao == 't') ? 'style="display:none;"' : ''; ?> >
            <table class="table table-hover dataTable">
                <thead>
                <tr>
                    <th></th>
                    <th>CPF</th>
                    <th>Nome</th>
                    <th>Email</th>
                    <th>Atuação</th>
                    <th>Cargo</th>
                </tr>
                </thead>
                <?php foreach ($arrConselheiros as $historico) : ?>
                    <tr>
                        <td>
                            <?php if($disabled == ''){?>
                            <a href="javascript:inativaConselheiro('<?php echo $historico['id']; ?>');"
                               title="Remover conselheiro">
                                <span class="btn btn-danger btn-xs glyphicon glyphicon-trash"></span>
                            </a>
                            <?php }?>
                        </td>
                        <td><?php echo formatar_cpf($historico['entcpf']); ?></td>
                        <td><?php echo $historico['entnome']; ?></td>
                        <td><?php echo $historico['entemail']; ?></td>
                        <td><?php echo $historico['entatuacao']; ?></td>
                        <td><?php echo $historico['entcargo']; ?></td>
                    </tr>
                <?php endforeach; ?>
            </table>
        </div>
    <?php else: ?>
        <div class="ibox-content">
            <?php
                switch ($itrid) {
                    case '1': $descricaoEsfera = 'estado';
                    break;
                    case '2': $descricaoEsfera = 'município';
                    break;
                    case '3': $descricaoEsfera = 'distrito';
                    break;
                }
            ?>
            <div class="alert alert-warning">Nenhum conselheiro vinculado para este <?php echo $descricaoEsfera; ?>.</div>
        </div>
    <?php endif; ?>
    <div class="ibox-footer">
        <div class="row">
            <div class="col-lg-6 text-left">
                <button type="button" class="btn btn-success previous" >Anterior</button>
            </div>
            <div class="col-lg-6 text-right">
                <button type="button" class="btn btn-success next" >Próximo</button>
            </div>
        </div>
    </div>
</div>

<div class="ibox collapsed" id="historico" <?php echo ($modelUnidade->inusemconselhoeducacao == 't') ? 'style="display:none;"' : ''; ?>>
    <div class="ibox-title">
        <h5>Conselheiros <?php echo $esfera; ?> de Educação - Histórico</h5>
        <div class="ibox-tools">
            <a class="collapse-link">
                <i class="fa fa-chevron-down"></i>
            </a>
        </div>
    </div>
    <?php $controllerInstrumentoUnidadeEntidade->formHistorico($arrParamHistorico); ?>
</div>

<div class="ibox float-e-margins animated modal" id="modal" tabindex="-1" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
        <form method="post" name="formulario" id="formulario" class="form form-horizontal">
            <input type="hidden" name="inuid" id="inuid" value="<?php echo $inuid ?>"/>
            <input type="hidden" name="tenid" id="tenid" value="<?php echo $tenid; ?>"/>
            <input type="hidden" name="req" value="salvarConselho"/>
            <div class="modal-content animated flipInY">
                <div class="ibox-title esconde " tipo="integrantes">
                    <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                    <h3>Conselheiros de Educação</h3>
                </div>
                <div class="ibox-content">
                    <?php $renderConselho->formConselho($disabled, $objPessoaFisica); ?>
                </div>
                <div class="ibox-footer">
                    <div class="col-lg-offset-3">
                        <button type="submit" class="btn btn-success salvar" <?php echo $disabled; ?>><i class="fa fa fa-plus-square-o"></i> Incluir conselheiro</button>
                    </div>
                </div>
            </div>
        </form>
    </div>
</div>
<script>
    function inativaConselheiro(id) {
        var inuid = $('#inuid').val();
        var url = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=' + inuid + '&menu=conselho';
        var action = '&req=removerConselheiro';
        var param = '&entid=' + id + '&tenid=' + $('#tenid').val();
        window.location.href = url + action + param;
    }

    $(document).ready(function() {
        //@TODO Gato pq exclusivamente essa funcionalidade nao funciona nessa tela, alguem remova umd ia
        $('.collapse-link').click( function() {
            
            var ibox = $(this).closest('div.ibox');
            var button = $(this).find('i');
            var content = ibox.find('div.ibox-content');
            content.slideToggle(200);
            button.toggleClass('fa-chevron-up').toggleClass('fa-chevron-down');
            ibox.toggleClass('').toggleClass('border-bottom');
            setTimeout(function () {
                ibox.resize();
                ibox.find('[id^=map-]').resize();
            }, 50);
        });
    
        $('.previous').click(function(){
            var url = window.location.href.toString().replace('conselho', 'comite');
            window.location.href = url;
        });

        $('.next').click(function(){
            var url = window.location.href.toString().replace('conselho', 'cae');
            window.location.href = url;
        });

        $('.sem_conselho').click(function(){

        	$.ajax({
                type: "POST",
                url: window.location.href,
                data: '&req=sem_conselho&inuid=<?php echo $_REQUEST['inuid']; ?>',
                async: true,
                beforeSend: function () {
                    $('#loading').show();
                },
                success: function (resp) {
                    if(resp == 'TRUE'){
                        $('#btnInserir').hide();
                        $('#lista').hide();
                        $('#historico').hide();
                    }else{
                        $('#btnInserir').show();
                        $('#lista').show();
                        $('#historico').show();
                    }
                    swal('Pronto!', 'Alteraçoes feitas com sucesso!', 'success');
                    window.location.href = window.location.href;
                }
            });

        });
    });
</script>