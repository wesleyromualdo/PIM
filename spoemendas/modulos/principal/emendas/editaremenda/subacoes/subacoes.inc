<div class="col-md-12">
    <?php
    /**
     * Sistema de Publicação.
     * werteralmeida
     */
    $dados['emeid'] = $_REQUEST['emeid'];
#ver($dados,d);
    /*
     * Beneficiários
     */
    $sql = "SELECT DISTINCT
                edeid,
                enb.enbcnpj,
                enb.enbnome,
                emd.gndcod,
                emd.mapcod,
                (SELECT COUNT(0) 
                FROM
                    emenda.iniciativadetalheentidade
                WHERE
                    idestatus = 'A'
                AND edeid = ede.edeid ) as iniciativas,
                edeid as indicadas,
                ede.edevalor
            FROM
                emenda.emendadetalhe emd
            JOIN
                emenda.emendadetalheentidade ede
            USING
                (emdid)
            JOIN
                emenda.entidadebeneficiada enb
            USING
                (enbid)
                WHERE emeid = {$dados['emeid']}
                AND enb.enbcnpj <> ''
                AND enbstatus = 'A'
                and emdstatus = 'A'
                AND edestatus = 'A' 
            ";
    $arrColunas = array('CNPJ', 'Beneficiário', 'GND', 'Modalidade','Iniciativas Indicadas', 'Subações Indicadas', 'Valor RCL (R$)');
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj');
    $listagem->addCallbackDeCampo('edevalor', 'mascaraMoeda');
    $listagem->addCallbackDeCampo(array('enbnome'), 'alinhaParaEsquerda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $permissaoEdicao = new Spoemendas_Controller_Permissaoedicaoemenda($_REQUEST['emeid']);
    $podeSalvar = $permissaoEdicao->geraPermissoesEdicao();
    if ($podeSalvar) {
        $listagem->addAcao('edit', array('func'=>'formVinculaSubAcoes', 'extra-params'=>array('mapcod')));
    } else {
        $listagem->esconderColunas('edeid');
    }
    $listagem->addCallbackDeCampo(array('indicadas'), 'detalharSubacoesPAR');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('edevalor'));
    $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    ?>
</div>
<script>
    $( document ).ready(function() {
        $('#btn-salvar-subacao-par').click(function(){
            $('#subacoes-par').submit();
            }
        );
    });
    /*
     *  Chamada da modal para Editar as iniciativas.
     */
    function formVinculaSubAcoes(edeid, mapcod) {
        
        $.ajax(window.location.href + '&requisicao=retornar-subacoes-par-ajax&edeid=' + edeid+'&mapcod='+mapcod).done(function (res) {
            $('#form-subacoes-par .modal-body').html(res);
            $('#form-subacoes-par').modal();
            setTimeout(function(){
                $("#ppsid").chosen();
            }, 200);
        });
    }
</script>
<div class="modal fade" id="form-subacoes-par">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Subações do PAR para o Beneficiário</h4>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-primary" id="btn-salvar-subacao-par" >Salvar</button>
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>
