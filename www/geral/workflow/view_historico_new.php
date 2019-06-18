<?php

function listaPermissoesTramitacao($aedid, $query)
{
    function formatarTelefone($usufoneddd, $dados)
    {
        return "({$usufoneddd}) {$dados['usufonenum']}";
    }

    function formatarLocalizacao($codlocalizacao, $dados)
    {
        return <<<HTML
<abbr data-toggle="popover" title="{$dados['dsclocalizacao']}">{$codlocalizacao}</abbr>
HTML;
    }

    $listagem = new Simec_Listagem($tipoRelatorio = Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo(array('usunome', 'usuemail', 'pfldsc'), 'alinhaParaEsquerda')
        ->addCallbackDeCampo('usufoneddd', 'formatarTelefone')
        ->addCallbackDeCampo('codlocalizacao', 'formatarLocalizacao')
        ->esconderColunas('usufonenum', 'dsclocalizacao')
        ->setCabecalho(array('Perfil', 'Nome', 'E-mail', 'Telefone', 'Localização'))
        ->setQuery($query);

    return $listagem->render();
}

function retornaHistoricoBootsrap($docid)
{
    function formatarLocalizacao($unicod, $dados)
    {
        return <<<HTML
<button type="button" class="btn btn-default" data-container="body" data-toggle="popover" data-placement="left" data-content="{$dados['unidsc']}">{$unicod}</button>
HTML;
    }
    function formatarEstadoDocumento($esddsc, $dados, $hstid)
    {
        $esddsc = alinharEsquerda($esddsc);
        $dados['cmddsc'] = simec_htmlspecialchars(($dados['cmddsc']));

        return <<<HTML
<span id="hstid_{$hstid}" data-comentario="{$dados['cmddsc']}">{$esddsc}</span>
HTML;
    }

    $sql = <<<DML
SELECT hd.hstid,
       ed.esddsc,
       ac.aeddscrealizada,
       us.usunome,
       TO_CHAR(hd.htddata, 'DD/MM/YYYY HH24:MI:SS') as datahora,
       hd.docid,
       us.unicod,
       un.unidsc,
       COALESCE(cd.cmddsc, '-') AS cmddsc
  FROM workflow.historicodocumento hd
    INNER JOIN workflow.acaoestadodoc ac ON (ac.aedid = hd.aedid)
    INNER JOIN workflow.estadodocumento ed ON (ed.esdid = ac.esdidorigem)
    INNER JOIN seguranca.usuario us ON (us.usucpf = hd.usucpf)
    LEFT JOIN workflow.comentariodocumento cd ON (cd.hstid = hd.hstid)
    LEFT JOIN public.unidade un USING(unicod)
  WHERE hd.docid = {$docid}
  ORDER BY hd.htddata ASC,
           hd.hstid ASC
DML;
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $cabecalho = array(
        'Onde Estava',
        'O que aconteceu',
        'Quem fez',
        'Quando fez',
        'Localização'
    );
    $listagem->addCallbackDeCampo('aeddscrealizada', 'alinhaParaEsquerda')
        ->addCallbackDeCampo('esddsc', 'formatarEstadoDocumento');
    $listagem->addCallbackDeCampo('unicod', 'formatarLocalizacao');
    $listagem->setCabecalho($cabecalho);
    $listagem->setQuery($sql);
    $listagem->addAcao('comment', 'historicoBootstrapComentario')
        ->setAcaoComoCondicional('comment', array(array('op' => 'diferente', 'campo' => 'cmddsc', 'valor' => '-')));
    $listagem->esconderColunas('docid', 'unidsc', 'cmddsc');
    $tabela = $listagem->render();

    $tabela .= <<<HTML
<script type="text/javascript">

    function historicoBootstrapComentario(hstid)
    {
        var id = '#arow-' + hstid;
        var \$parentTr = $(id).closest('tr');
    
        if ($(id + ' span').hasClass('open')) {
            \$parentTr.next().remove();
            $(id + ' span').removeClass('open').removeClass('btn-default').addClass('btn-info');
        } else {
            $(id + ' span').addClass('open').removeClass('btn-info').addClass('btn-default');;
            var numCols = $('td', \$parentTr).length;
            numAcao = \$parentTr.parents('table').attr('data-qtd-acoes');
            td_acao = '<td colspan="'+ numAcao +'">&nbsp;</td>';
            \$parentTr.after('<tr>'+td_acao+'<td colspan="' + numCols + '"><blockquote style="text-align:left">' + $('#hstid_' + hstid).attr('data-comentario') + '</blockquote></td></tr>');
        }
    }
    
    $(function(){
        $('#modal-alert .modal-dialog').addClass('modal-lg');
        $('[data-toggle="popover"]').popover({placement:'top',trigger:'hover'});
    });

</script>
HTML;
    return $tabela;
}

$requisicao = $_REQUEST['requisicao'];
include APPRAIZ . "includes/funcoesspo_componentes.php";
include APPRAIZ . "includes/funcoesspo.php";
include_once APPRAIZ . 'includes/library/simec/Listagem.php';

switch ($requisicao) {
    case 'informacaoTramitacao':
        require_once(APPRAIZ . "/www/{$_SESSION['sisdiretorio']}/_funcoes.php");
        require_once(APPRAIZ . "/www/{$_SESSION['sisdiretorio']}/_constantes.php");

        $aedid = $_REQUEST['aedid'];
        $docid = $_REQUEST['docid'];
        $sql = "SELECT aeddescregracondicao FROM workflow.acaoestadodoc WHERE aedid = {$aedid}";
        $conteudo = $db->pegaUm($sql);
        ?>

        <div class="panel-group" id="acc-tramitacao" role="tablist" aria-multiselectable="true">

            <?php
            echo montaItemAccordion(
                "Observações sobre a Tramitação",
                'observacoes',
                "<br/><span style=\"text-align:left !important;\">{$conteudo}</span><br/><br/>",
                array(
                    'aberto' => true,
                    'accordionID' => 'acc-tramitacao'
                )
            );

            if (is_callable('wf_pessoasQuePodemTramitar')) {
                $sql = sprintf($_SESSION['sqlPessoasTramitar'], $aedid);
                $conteudo = listaPermissoesTramitacao($aedid, wf_pessoasQuePodemTramitar($aedid, $docid));
                echo montaItemAccordion(
                    "Pessoas que podem realizar essa transação",
                    'pessoas',
                    $conteudo,
                    array('accordionID' => 'acc-tramitacao')
                );
            }
            ?>
        </div>

        <script type="text/javascript">
            $(function () {
                $('#modal-alert .modal-dialog').css('width', '70%');
                $('[data-toggle="popover"]').popover({placement: 'top', trigger: 'hover'});
            });
        </script>

        <?php
        die;
    case 'historicoBootstrap':
        $docid = $_REQUEST['docid'];
        echo retornaHistoricoBootsrap($docid);
        die;
}