<?php
/**
 * @TODO REFATORAR QUERIES EM MODELS - Verificar antes se já não existem lá
 */


include_once APPRAIZ . 'spoemendas/classes/model/Emendaimpedimentobeneficiario.class.inc';
include_once APPRAIZ . 'spoemendas/classes/model/Solicitacaofinanceira.class.inc';

/**
 * Função de Callback do campo de progresso da coluna.
 *
 * @user Lindalberto Filho <lindalbertorvcf@gmail.com>
 *
 * @param integer $valor
 *
 * @return string
 */
function callbackProgressBar ($valor)
{
    return outputBar(true, ['class' => 'progress-bar progress-bar-info progress-bar-striped active', 'value' => $valor, 'spClass' => '']);
}

/**
 * Verifica se a requisição é ajax.
 * Is request xmlHttpRequest
 *
 * @return bool
 */
function isAjax ()
{
    return (!empty($_SERVER['HTTP_X_REQUESTED_WITH']) &&
        strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest');
}

function painelInformacoesEmendaModal ($emeid, $colunas = 8)
{
    global $informacoes;
    $dados = retornaDadosEmenda($emeid);
    $perfis = pegaPerfilGeral($_SESSION['usucpf']);
    $planoOrcamentario = pegaPlanoOrcamentarioEmenda($emeid);

    $meia_coluna = $colunas / 2;
    echo <<<HTML

<div class="col-md-{$colunas} col-md-offset-1">
    <div class="panel panel-info">
        <div class="panel-heading">
            <h3 class="panel-title">Emenda Parlamentar nº <b>{$dados['emecod']} {$planoOrcamentario}</b></h3>
        </div>
        <div class="panel-body">
        <div class="col-md-{$meia_coluna} ">
        <table cellspacing="1" cellpadding="3" align="center" class="table table-striped table-bordered table-hover">
            <tbody>
                <tr>
                    <td >Autor:</td>
                    <td><b style="font-size:14px">{$dados['autor']}</b></td>
                </tr>
                <tr>
                    <td >Unidade Orçamentária (UO):</td>
                    <td>{$dados['unidade']}</td>
                </tr>
                <tr>
                    <td >Responsável:</td>
                    <td>{$dados['responsavel']}</td>
                </tr>
                <tr>
                    <td >Ação:</td>
                    <td>{$dados['acao']}</td>
                </tr>
            </tbody>
        </table>
        </div>
        <div class="col-md-{$meia_coluna} col-md-offset-1">
        <table cellspacing="1" cellpadding="3" align="center" class="table table-striped table-bordered table-hover">
            </tbody>
                <tr>
                    <td colspan=2><center><b>Financeiro</b></center></td>
                </tr>
                     <tr>
                    <td >Dotação Atualizada (R$):</td>
                    <td><b>{$dados['valoremenda']}</b></td>
                </tr>
                    <tr>
                    <td >Limite de Empenho (R$):</td>
                    <td>{$dados['limite']}</td>
               </tr>
                    <tr>
                    <td >Empenhado (R$):</td>
                    <td>{$dados['empenhado']}</td>
                </tr>
            </tbody>
        </table>
        </div>
    </div>
    </div>
</div>

<div class="col-md-2">
    <div class="form-group">
        <button class="btn btn-warning"
                onClick="javascript:imprimirModalEmenda({$dados['emeid']})"
                type="button"><span class="glyphicon glyphicon-print"></span> Imprimir
        </button>
    </div>
</div>
<br style="clear:both" />
HTML;
}

/*
 * Retorna uma modal, apenas de leitura, com os dados da Emenda
 * Parâmetro obrigatório: ['emeid']
 */

function retornaModalDetalheEmenda ($params)
{
    global $db;
    $naoEncontrado = '
        <br/><div class="alert alert-danger text-center col-md-8 col-md-offset-2">
        <button class="close" aria-hidden="true" data-dismiss="alert" type="button">×</button>
        Nenhum registro encontrado.
        </div>';
    /*
     * Dados orçamentários da Emenda
     */
    painelInformacoesEmendaModal($params['emeid'], 10);
    $dados = retornaDadosEmenda($params['emeid']);
    $perfis = pegaPerfilGeral();
    $saida .= "<br/><br/>";
    /*
     *  Detalhes da Emenda
     */
    $sql = "SELECT DISTINCT
                gndcod,
                mapcod,
                foncod,
                emdvalor
            FROM
                emenda.emendadetalhe ed
            WHERE
                emeid = {$params['emeid']} and emdstatus = 'A'
            ";

    $arrColunas = ['GND', 'Modalidade', 'Fonte', 'Valor (R$)'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo(['emdvalor', edevalordisponivel], 'mascaraMoeda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['edevalor']);
    $dadosBeneficiarios = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    $saida .= montaItemAccordion(
        '<span class="glyphicon glyphicon-th-large"></span> Detalhes da Emenda', 'dadosBeneficiarios', "{$dadosBeneficiarios}", ['accordionID' => 'dadosBeneficiarios',
                                                                                                                                 'aberto'      => true,
                                                                                                                                 'decoracao'   => 'info']
    );
    /*
     *  Beneficiários
     */
    $sql = "SELECT DISTINCT
                enb.enbcnpj,
                enb.enbnome,
                emd.gndcod,
                emd.mapcod,
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
                WHERE emeid = {$params['emeid']}
                AND enbstatus = 'A'
                and emdstatus = 'A'
                AND edestatus = 'A'
            ";
    $arrColunas = ['CNPJ', 'Beneficiário', 'GND', 'Modalidade', 'Valor RCL (R$)'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj');
    $listagem->addCallbackDeCampo(['edevalor', 'edevalordisponivel'], 'mascaraMoeda');
    $listagem->addCallbackDeCampo(['enbnome'], 'alinhaParaEsquerda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    //$listagem->addAcao('view', 'verDetalheBeneficiario');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['edevalor']);
    $dadosBeneficiarios = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    $saida .= montaItemAccordion(
        '<span class="glyphicon glyphicon-home"></span> Beneficiários', 'dadosBeneficiarios', "{$dadosBeneficiarios}", ['accordionID' => 'dadosBeneficiarios',
                                                                                                                        'aberto'      => true,
                                                                                                                        'decoracao'   => 'info']
    );
    /**
     * Plano de Trabalho / Impedimentos
     */
    $arrColunas = ['CNPJ', 'Beneficiário', 'GND', 'Modalidade', 'Fonte', 'Impedimento', 'Detalhe do Plano de Trabalho / Impedimentos', 'Valor Impedido(R$)'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj')
        ->addCallbackDeCampo(['enbnome', 'edidescricao'], 'alinhaParaEsquerda')
        ->addCallbackDeCampo('edivalor', 'mascaraMoeda')
        ->addCallbackDeCampo('ediimpositivo', 'formatarImpeditivo')
        ->esconderColunas('ediid', 'eibid', 'edeid', 'ireid', 'eibdscobj', 'emiid')
        ->setCabecalho($arrColunas)
        ->setQuery((new Spoemendas_Model_Emendaimpedimentobeneficiario())
            ->pegaImpedimentos(['emeid' => $params['emeid']], true)
        )->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['edivalor']);
    $listagem->esconderColunas('emdid', 'edeprioridade', 'valor_entidade', 'edelimiteempenhobeneficiario');
    $dadosImpedimentos = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);

    $saida .= montaItemAccordion(
        '<span class="glyphicon glyphicon-th-list"></span> Plano de Trabalho / Impedimentos', 'dadosImpedimentos', "{$dadosImpedimentos}", ['accordionID' => 'dadosBeneficiarios',
                                                                                                                                            'aberto'      => true,
                                                                                                                                            'decoracao'   => 'info']
    );


    /*
     * Iniciativas por Beneficiários
     */
    $sql = "SELECT DISTINCT
                enb.enbcnpj,
                enb.enbnome,
                emd.gndcod,
                emd.mapcod,
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
                WHERE emeid = {$params['emeid']}
                     AND enb.enbcnpj <> ''
                     AND enbstatus = 'A'
                     and emdstatus = 'A'
                     AND edestatus = 'A'

            ";
    $arrColunas = ['CNPJ', 'Beneficiário', 'GND', 'Modalidade', 'Iniciativas Indicadas', 'Valor RCL (R$)'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj');
    $listagem->addCallbackDeCampo(['edevalor', 'edevalordisponivel'], 'mascaraMoeda');
    $listagem->addCallbackDeCampo(['enbnome'], 'alinhaParaEsquerda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $listagem->addCallbackDeCampo(['indicadas'], 'detalharIniciativaBeneficiario');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['edevalor']);
    $dadosIndicacoes = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    $saida .= montaItemAccordion(
        '<span class="glyphicon glyphicon-tag"></span> Iniciativas da emenda', 'dadosIndicacoes', "{$dadosIndicacoes}", ['accordionID' => 'dadosIndicacoes',
                                                                                                                         'aberto'      => true,
                                                                                                                         'decoracao'   => 'info']
    );
    /*
     * Subações do PAR por beneficiário
     */
    $sql = "SELECT DISTINCT
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
                WHERE emeid = {$params['emeid']}
                 AND enb.enbcnpj <> ''
                     AND enbstatus = 'A'
                     and emdstatus = 'A'
                     AND edestatus = 'A'
            ";
    $arrColunas = ['CNPJ', 'Beneficiário', 'GND', 'Modalidade', 'Iniciativas Indicadas', 'Subações Indicadas', 'Valor RCL (R$)'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj');
    $listagem->addCallbackDeCampo('edevalor', 'mascaraMoeda');
    $listagem->addCallbackDeCampo(['enbnome'], 'alinhaParaEsquerda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $listagem->addCallbackDeCampo(['indicadas'], 'detalharSubacoesPAR');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['edevalor']);
    /* Apenas para emendas do PAR */
    if ($dados['etoid'] == 1)
    {
        $dadosSubacoes = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
        $saida .= montaItemAccordion(
            '<span class="glyphicon glyphicon-ok-sign"></span> Subações do PAR', 'dadosSubacoes', "{$dadosSubacoes}", ['accordionID' => 'dadosSubacoes',
                                                                                                                       'aberto'      => true,
                                                                                                                       'decoracao'   => 'info']
        );
    }
    /*
     *  Plano de Trabalho (Emendas / PAR)
     */
    $sql = "SELECT DISTINCT
                enb.enbcnpj,
                enb.enbnome,
                emd.gndcod,
                emd.mapcod,
                ede.edevalor,
                COALESCE(esd.esddsc,'Não iniciado.') as esddsc
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
            LEFT JOIN
                emenda.ptemendadetalheentidade pte
            USING
                (edeid)
            LEFT JOIN
                emenda.planotrabalho ptr
            USING
                (ptrid)
            LEFT JOIN
                workflow.documento doc
            USING
                (docid)
            LEFT JOIN
                workflow.estadodocumento esd
            USING
                (esdid)
            WHERE
                emeid = {$params['emeid']}
                     AND enbstatus = 'A'
                     and emdstatus = 'A'
                     AND edestatus = 'A'
            ";
    $arrColunas = ['CNPJ', 'Beneficiário', 'GND', 'Modalidade', 'Valor RCL (R$)', 'Situação'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo('enbcnpj', 'formatarcnpj');
    $listagem->addCallbackDeCampo('edevalor', 'mascaraMoeda');
    $listagem->addCallbackDeCampo(['enbnome'], 'alinhaParaEsquerda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['edevalor']);
    $dadosPlanotrabalho = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    $saida .= montaItemAccordion(
        '<span class="glyphicon glyphicon-book"></span> Plano de Trabalho (Emendas / PAR)', 'dadosPlanotrabalho', "{$dadosPlanotrabalho}", ['accordionID' => 'dadosPlanotrabalho',
                                                                                                                                            'aberto'      => true,
                                                                                                                                            'decoracao'   => 'info']
    );

    /*
     *  NLs
     */
    $sql = "SELECT
                enl.nl,
                to_char(enl.nldata, 'dd/mm/yyyy') as nldata,
                enl.nlvalor
            FROM
                spoemendas.emendanl enl
            WHERE
                enl.emecod = '{$dados['emecod']}'
            AND enl.exercicio = '{$dados['emeano']}'
            ORDER BY
                enl.nldata DESC
            ";
    $arrColunas = ['NL', 'Data', 'Valor (R$)'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo(['nlvalor'], 'mascaraMoeda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['nlvalor']);
    $dadosNL = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    $saida .= montaItemAccordion(
        '<span class="glyphicon glyphicon-usd"></span> Notas de Lançamento (NL)', 'dadosNL', "{$dadosNL}", ['accordionID' => 'dadosNL',
                                                                                                            'aberto'      => true,
                                                                                                            'decoracao'   => 'info']
    );
    /*
     *  NEs
     */
    $sql = "SELECT
                ene.ne,
                to_char(ene.nedata, 'dd/mm/yyyy') as nedata,
                ene.nevalor
            FROM
                spoemendas.emendane ene
            WHERE
                ene.emecod = '{$dados['emecod']}'
            AND ene.exercicio = '{$dados['emeano']}'
            ORDER BY
                ene.nedata DESC
            ";
    $arrColunas = ['NE', 'Data', 'Valor (R$)'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo(['nevalor'], 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['nevalor']);
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $dadosNE = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    $saida .= montaItemAccordion(
        '<span class="glyphicon glyphicon-usd"></span> Notas de Empenho (NE)', 'dadosNE', "{$dadosNE}", ['accordionID' => 'dadosNE',
                                                                                                         'aberto'      => true,
                                                                                                         'decoracao'   => 'info']
    );
    if (in_array(PFL_SUPER_USUARIO, $perfis) ||
        in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) ||
        in_array(PFL_CGF_EQUIPE_FINANCEIRA, $perfis) ||
        in_array(PFL_ASPAR, $perfis)
    )
    {
        /*
         *  Pedidos de Financeiro
         */

        $sql = "SELECT
                    TO_CHAR( lfn.lfninclusao , 'dd/mm/yyyy' ) as datapedido,
                    ung.ungcod || ' - ' || ung.ungdsc as ungdsc,
                    enb.enbnome || ' ( ' || enb.enbcnpj  || ' )'  as beneficiario,
                    lfn.lfnvalorsolicitado,
                    esddsc
           FROM progfin.liberacoesfinanceiras lfn
             LEFT JOIN progfin.pedidoliberacaofinanceira plf ON (plf.plfid = lfn.plfid)
             LEFT JOIN public.unidadegestora ung ON lfn.ungcodfavorecida = ung.ungcod
             LEFT JOIN public.unidade uni ON ung.unicod = uni.unicod
             LEFT JOIN progfin.classificacaopedido cp ON (cp.clpid = lfn.clpid)
             LEFT JOIN workflow.documento doc ON (doc.docid = plf.docid)
             LEFT JOIN workflow.documento doc2 ON (lfn.docid = doc2.docid)
             LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc2.esdid
             JOIN
                 emenda.emendadetalheentidade ede
             USING
                 (edeid)
             JOIN
                 emenda.entidadebeneficiada enb
             USING
                 (enbid)
           WHERE emeid = {$dados['emeid']}
           ORDER BY 3,4,5
            ";
        $arrColunas = ['Data do Pedido', 'Unidade Gestora (UG)', 'Beneficiário', 'Valor (R$)', 'Situação'];
        $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
        $listagem->addCallbackDeCampo(['lfnvalorsolicitado'], 'mascaraMoeda');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['lfnvalorsolicitado']);
        $listagem->setCabecalho($arrColunas)->setQuery($sql);
        $dadosFinanceiro = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
        $saida .= montaItemAccordion(
            '<span class="glyphicon glyphicon-usd"></span> Pedidos de Financeiro', 'dadosFinanceiro', "{$dadosFinanceiro}", ['accordionID' => 'dadosFinanceiro',
                                                                                                                             'aberto'      => true,
                                                                                                                             'decoracao'   => 'info']
        );
    }
    $sqlOficio = (new Spoemendas_Model_Emenda)->oficiosEmenda($dados['emeid']);
    $arrColunasOficio = ['Solicitante', 'Tipo Solicitação', 'Quem anexou', 'Data e Hora do Anexo', 'Status do anexo'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, ['lfnvalorsolicitado']);
    $listagem->setCabecalho($arrColunasOficio)->setQuery($sqlOficio);
    $dadosOficio = $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
    $saida .= montaItemAccordion(
        '<span class="glyphicon glyphicon-file"></span> Ofícios', 'dadosoficios', "{$dadosOficio}", ['accordionID' => 'dadosoficios',
            'aberto'      => true,
            'decoracao'   => 'info']
    );
    return $saida;
}

/*
 * Retorna as subações da iniciativa (detalhamento)
 */

function retornarDetalharIniciativa ($params)
{
    global $db;
    $params['iedid'] = $params['dados'][0];
    $params['emeid'] = $params['dados'][1];
    $sql = "SELECT DISTINCT
                esp.espnome,
                esp.espunidademedida
            FROM
                emenda.emendadetalhe emd
            JOIN
                emenda.iniciativaemendadetalhe ind
            USING
                (emdid)
            JOIN
                emenda.iniciativaespecificacao ies
            USING
                (iniid)
            JOIN
                emenda.especificacao esp
            USING
                (espid)
            WHERE
                esp.espstatus = 'A'
            AND emd.emeid = {$params['emeid']}
            AND ind.iedid = {$params['iedid']}
            and emdstatus = 'A'
            ORDER BY
                1
            ";
#ver($sql);
    $arrColunas = ['Subação', 'Unidade de Medida'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo(['espnome'], 'alinhaParaEsquerda');
    $listagem->setCabecalho($arrColunas)->setQuery($sql);

    return $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}

;

function tratarSimNao ($texto)
{
    switch ($texto)
    {
        case 'N':
            return '<span class="label label-danger">Não</span>';
        case 'S':
            return '<span class="label label-success">Sim</span>';
    }

    return $texto;
}

function formatarcnpj ($cnpj)
{
    return formatar_cnpj($cnpj);
}

function formatarcpf ($cpf)
{
    return formatar_cpf($cpf);
}

/**
 * Formata o código da UO, para que apareça a sigla e no tooltip o nome da UO
 *
 * @param string $unidade Código do PI para formatação.
 */
function formatarUnidadeListagem ($unidade)
{
    if (empty($unidade))
    {
        return '-';
    }
    list($unicod, $uniabrev, $unidsc) = explode(' - ', $unidade);

    return <<<HTML
<abbr data-toggle="tooltip" data-placement="top"  title="{$unidsc}">{$unicod} - {$uniabrev}</abbr>
HTML;
}

/**
 * Formata o código da acao, para que apareça a sigla e no tooltip o nome da acao
 *
 * @param string $acao texto para formatação.
 */
function formatarAcaoListagem ($acao)
{
    if (empty($acao))
    {
        return '-';
    }
    list($acacod, $acadsc) = explode(' - ', $acao);

    return <<<HTML
<abbr data-toggle="tooltip" data-placement="top"  title="{$acadsc}">{$acacod}</abbr>
HTML;
}

/**
 * Painel de informaões gerais sobre a Emenda
 *
 * @param $emeid
 */
function painelInformacoesEmenda ($emeid, $colunas = 8)
{
    global $informacoes;
    $dados = retornaDadosEmenda($emeid);
    $planoOrcamentario = pegaPlanoOrcamentarioEmenda($emeid);
    $financeiro = <<<HTML
        <tr>
                    <td >Cancelado (R$): <b>{$dados['cancelado']}</b></td>
                </tr>
                    <tr>
                    <td >Pago(R$): <b>{$dados['pago']}</b></td>
                </tr>
                <tr>
                    <td >Saldo a pagar (R$): <b>{$dados['saldoapagar']}</b></td>
                </tr>
HTML;

    // Retira o financeiro (menos para SPO) para exercicio < 2015
    if ($dados['exibevaloresfinanceiros'] != 'S')
    {
        $financeiro = "";
    }
    $meia_coluna = $colunas / 2;
    echo <<<HTML

<div class="col-md-{$colunas}">
    <div class="panel panel-info">
        <div class="panel-heading">
            <h4 class="panel-title">Emenda Parlamentar nº <b>{$dados['emecod']} {$planoOrcamentario}</b></h4>
        </div>
        <table cellspacing="1" cellpadding="3" align="center" class="table table-striped table-bordered table-hover">
            <tbody>
                <tr>
                    <td>Autor: <b style="font-size:14px">{$dados['autor']}</b></td>
                </tr>
                <tr>
                    <td>Unidade Orçamentária (UO): <strong>{$dados['unidade']}</strong></td>
                </tr>
                <tr>
                    <td>Responsável: <strong>{$dados['responsavel']}</strong></td>
                </tr>
                <tr>
                    <td>Ação: <strong>{$dados['acao']}</strong></td>
                </tr>
                <tr>
                    <td style="background: #d9edf7; color: #3a87ad; border-color: #bce8f1;" colspan=2><center><b>Financeiro</b></center></td>
                </tr>
                 <tr>
                    <td>Dotação Atualizada (R$): <b>{$dados['valoremenda']}</b></td>
                </tr>
                <tr>
                    <td>Limite de Empenho (R$): <b>{$dados['limite']}</b></td>
                </tr>
                <tr>
                    <td>Empenhado (R$): <b>{$dados['empenhado']}</b></td>
                </tr>
                {$financeiro}
            </tbody>
        </table>
    </div>

HTML;

    echo <<<HTML
</div>
HTML;
}


/**
 * Painel de informaões gerais sobre a Emenda
 *
 * @param $emeid
 */
function painelInformacoesEmendaParlamentar ($emeid, $colunas = 4, $colunasAbas = 8)
{
    global $informacoes;
    $dados = retornaDadosEmenda($emeid);
    $meia_coluna = $colunas / 2;

    $planoOrcamentario = pegaPlanoOrcamentarioEmenda($emeid);

    echo <<<HTML

<div class="col-md-{$colunas}">
    <div class="panel panel-info">
        <div class="panel-heading">
            <h4 class="panel-title">Emenda Parlamentar nº <b>{$dados['emecod']} {$planoOrcamentario}</b></h4>
        </div>
        <div>
            <div class="row">
                <table cellspacing="1" cellpadding="3" align="center" class="table table-striped table-bordered table-hover">
                    <tbody>
                        <tr>
                            <td>Autor: <b style="font-size:14px">{$dados['autor']}</b></td>
                        </tr>
                        <tr>
                            <td>Exercício: <b style="font-size:14px">{$dados['emeano']}</b></td>
                        </tr>
                        <tr>
                            <td>Unidade Orçamentária (UO): <b style="font-size:14px">{$dados['unidade']}</b></td>
                        </tr>
                        <tr>
                            <td>Responsável: <b style="font-size:14px">{$dados['responsavel']}</b></td>
                        </tr>
                        <tr>
                            <td>Ação: <b style="font-size:14px">{$dados['acao']}</b></td>
                        </tr>
                        <tr>
                            <td  style="background: #d9edf7; color: #3a87ad; border-color: #bce8f1;"  colspan=2><center><b>Financeiro</b></center></td>
                        </tr>
                        <tr>
                            <td>Dotação Atualizada (R$): <b>{$dados['valoremenda']}</b></td>
                        </tr>
                        <tr>
                            <td>Limite de Empenho (R$): <b>{$dados['limite']}</b></td>
                        </tr>
                        <tr>
                            <td>Empenhado (R$): <b>{$dados['empenhado']}</b></td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
    </div>
</div>
<div class="col-md-8">
HTML;
}


/*
 *  Retorna os dados de uma emenda através do EMEID
 */

function retornaDadosEmenda ($emeid)
{
    global $db;
    $sql = "SELECT
                eme.emeid,
                eme.emecod,
                uni.unicod || ' - ' || uni.unidsc AS unidade,
                vfp.fupfuncionalprogramaticacompleta,
                eme.emeano,
                vfp.acacod || ' - ' || vfp.fupdsc AS acao,
                a.autnome || ' ('  || par.parsigla || ' / '  || a.estuf || ') ' AS autor,
                ere.resdsc as responsavel,
                eme.etoid,
                to_char(eme.emevalor, '9G999G999G990D99')                  AS valoremenda,
                ed.emdimpositiva,
                 CASE WHEN SUM(sex.vlrsaldoapagar + sex.vlrpago + sex.vlrempenhocancelado) > 0 THEN 'S' ELSE 'N' END as exibevaloresfinanceiros,
                to_char(COALESCE(SUM(DISTINCT sex.vlrlimiteempenho),0), '9G999G999G990D99')         AS limite,
                to_char(COALESCE(SUM(DISTINCT sex.vlrempenhado),0), '9G999G999G990D99')             AS empenhado,
                to_char(COALESCE(SUM(DISTINCT sex.vlrpago),0), '9G999G999G990D99')                  AS pago,
                to_char(COALESCE(SUM(DISTINCT sex.vlrpago),0), '9G999G999G990D99')                  AS pago,
                to_char(COALESCE(SUM(DISTINCT sex.vlrempenhocancelado),0), '9G999G999G990D99')      AS cancelado,
                to_char(COALESCE(SUM(DISTINCT sex.vlrsaldoapagar),0), '9G999G999G990D99')           AS saldoapagar,
                to_char(CEIL(
                    CASE
                        WHEN SUM(DISTINCT sex.vlrempenhado) > 0
                        THEN SUM(DISTINCT sex.vlrempenhado) * 100 / SUM(DISTINCT sex.vlrlimiteempenho)
                        ELSE 0
                    END), '9G999G999G990D99') AS limiteempenhado
            FROM
                emenda.emenda eme
            LEFT JOIN
                emenda.v_funcionalprogramatica vfp
            USING
                (acaid)
            LEFT JOIN
                public.unidade uni
            USING
                (unicod)
            LEFT JOIN
                emenda.emendadetalhe ed
            ON
                (
                    ed.emeid = eme.emeid) and emdstatus = 'A'
            LEFT JOIN
                emenda.emendadetalheentidade ede
            ON
                (
                    ede.emdid = ed.emdid
                AND edestatus = 'A')
            LEFT JOIN
                spo.siopexecucao sex
            ON
                sex.emecod::text = eme.emecod::text
            AND sex.exercicio::text = eme.emeano::text
             INNER JOIN
                emenda.autor a
            ON
                (
                    a.autid = eme.autid)
            JOIN emenda.partido par on a.parid = par.parid
            LEFT JOIN emenda.responsavel ere ON ere.resid = eme.resid
            WHERE
                eme.emeid = {$emeid}
            GROUP BY
                1,2,3,4,5,6,7,8,9,10, emdimpositiva";

    return $db->pegaLinha($sql);
}

function pegaPlanoOrcamentarioEmenda ($emeid)
{
    global $db;

    $sql = <<<SQL
        select 
            plocodigo
        from emenda.emenda eme 
        inner join monitora.planoorcamentario plo using(ploid)
        where emeid = %d;
SQL;

    $plocodigo = $db->pegaUm(sprintf($sql, $emeid));

    //@todo - Retirar esta parte do código quando o banco de produção for atualizado e todas as emendas possuírem o ploid
    if (empty($plocodigo)) {
        $strSQL = <<<DML
    SELECT
        eme.emecod, po.plocodigo, aut.autcod
    FROM emenda.emenda eme
    JOIN monitora.planoorcamentario po ON (po.acaid = eme.acaid)
    JOIN emenda.autor aut ON (aut.autid = eme.autid)
    WHERE po.plocodigo <> '0000' AND eme.emeid = {$emeid}
DML;

        if ($retorno = $db->pegaLinha($strSQL)) {
            switch (true) {
                case $retorno['autcod'] < 5000:
                    return 'EIND';
                case $retorno['autcod'] > 4999 && $retorno['autcod'] < 7000:
                    return 'EBAN - EREL';
                case $retorno['autcod'] > 6999 && $retorno['autcod'] < 8000:
                    return 'ECOM';
                case $retorno['autcod'] > 7999:
                    return 'EREL';
            }
        }
    } else {
        return $plocodigo;
    }
}

/*
 * Detalhar as iniciativas dobeneficiário na Tabela
 */

function detalharIniciativaBeneficiario ($edeid)
{
    global $db;

    $sql = "SELECT DISTINCT
                ininome,
                gndcod
            FROM
                emenda.iniciativadetalheentidade
            JOIN
                emenda.iniciativa
            USING
                (iniid)
            WHERE
                idestatus = 'A'
            AND edeid = {$edeid}
            ORDER BY
                1";
    $dados = $db->carregar($sql);
    $saida = "<p style=\"text-align: left !important;\">";
    if (is_array($dados))
    {
        foreach ($dados as $dado)
        {
            $saida .= "- {$dado['ininome']} (GND: {$dado['gndcod']}) <br/>";
        }
    }
    else
    {
        $saida .= "Nenhuma iniciativa indicada.";
    }
    $saida .= "</p>";

    #ver($saida);
    return $saida;
}

function detalharSubacoesPAR ($edeid)
{
    global $db;

    $sql = "SELECT DISTINCT
                par.retornacodigopropostasubacao(ps.ppsid)AS codigo,
                ps.ppsdsc                                 AS descricao
            FROM
                par.propostasubacao ps
            INNER JOIN
                par.propostasubacaoiniciativaemenda pse
            ON
                pse.ppsid = ps.ppsid
            INNER JOIN
                emenda.iniciativadetalheentidade ide
            ON
                ide.iniid = pse.iniid
            AND ide.idestatus = 'A'
            INNER JOIN
                par.indicador i
            ON
                i.indid = ps.indid
            AND i.indstatus = 'A'
            INNER JOIN
                par.area a
            ON
                a.areid = i.areid
            AND a.arestatus = 'A'
            INNER JOIN
                par.dimensao dim
            ON
                dim.dimid = a.dimid
            INNER JOIN
                par.criterio c
            ON
                c.indid = i.indid
            INNER JOIN
                par.propostaacao pa
            ON
                pa.crtid = c.crtid
            INNER JOIN
                emenda.emendapariniciativa ein
            ON
                ein.ppsid = ps.ppsid
            WHERE
                ein.edeid = {$edeid}
            ORDER BY
                1";
    $dados = $db->carregar($sql);
    if ($dados)
    {
        $saida = "<p style=\"text-align: left !important;\">";
        foreach ($dados ? $dados : [] as $dado)
        {
            $saida .= "- <abbr data-toggle=\"tooltip\" data-placement=\"top\"  title=\"{$dado['descricao']}\">{$dado['codigo']}</abbr> &nbsp;";
        }
        $saida .= "</p>";
    }

    #ver($saida);
    return $saida;
}

/*
 * Retorna tabela de iniciativas da emenda
 */

function retornaIniciativasEmenda ($emeid)
{
    /*
     *  Iniciativas
     */
    $sql = <<<DML
SELECT DISTINCT ini.iniid,
                ini.ininome,
                ini.gndcod,
                ini.iniresolucao
  FROM emenda.emendadetalhe emd
    JOIN emenda.iniciativaemendadetalhe ind USING(emdid)
    JOIN emenda.iniciativa ini USING(iniid)
  WHERE emeid = {$emeid}
    AND ini.inistatus = 'A'
  	and emdstatus = 'A'
    AND ind.iedstatus = 'A'
  ORDER BY gndcod
DML;

    $arrColunas = ['Iniciativa', 'GND', 'Resolução'];
    $listagem = new Simec_Listagem(SIMEC_LISTAGEM::RELATORIO_CORRIDO, SIMEC_LISTAGEM::RETORNO_BUFFERIZADO);
    $listagem->addCallbackDeCampo(['ininome'], 'alinhaParaEsquerda');
    $listagem->esconderColuna(['emeid']);
    $listagem->setCabecalho($arrColunas)->setQuery($sql);
    $dados = retornaDadosEmenda($emeid);
    /* Mostra apenas para emendas do PAR */
    if ($dados['etoid'] == 1)
    {
        $listagem->addAcao('view', 'verSubacoesParIniciativa');
    }
    else
    {
        $listagem->esconderColuna('iniid');
    }

    return $listagem->turnOnPesquisator()->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}

function formatarContato ($ddd, $dados)
{
    return "({$ddd}) {$dados['edetelresp']}<br />{$dados['edemailresp']}";
}

/**
 * Verifica se existe um docid e grava um novo, se necessário.
 *
 * @param mixed[]                            $informacoes Informações de uma emenda, incluíndo docid
 * @param Spoemendas_Model_Emendainformacoes $infoemenda
 * @param int                                $emeid
 */
function cadastrarDocid ($informacoes, $infoemenda, $emeid)
{
    if (empty($informacoes) && !empty($infoemenda))
    {
        $infoemenda->popularDadosObjeto([
            'emeid' => $emeid,
            'docid' => wf_cadastrarDocumento(WF_TPDID_SPOEMENDAS, 'Emendas parlamentares')
        ]);
        $infoemenda->salvar();
        $infoemenda->commit();
    }
}

/*
 * Retorna se pode ou não salvar, de acordo com o tipo e regras específicas
 */

function podeSalvar ($params)
{
    if (!EMENDA_IMPOSITIVA == retornaDadosEmenda($params['emeid'])['emdimpositiva'])
    {
        return false;
    }

    $dadosPeriodoAtual = retornaDadosPeriodoAtual();
    $formaEx = recuperaFormaExecucao($params['emeid']);
    $infoEmenda = new Spoemendas_Model_Emendainformacoes($params['einid']);


    // Prepara datas para comparação
    $dataIni = DateTime::createFromFormat('d/m/Y', $dadosPeriodoAtual[$formaEx]['prfdatainicio']);
    $dataFim = DateTime::createFromFormat('d/m/Y', $dadosPeriodoAtual[$formaEx]['prfdatafim']);
    $dataAtu = DateTime::createFromFormat('d/m/Y', date('d/m/Y'));

    // verifica se a data atual é menor que a data de inicio ou se a data atual
    // é maior que a data fim
    if ($dataAtu < $dataIni || $dataAtu > $dataFim)
    {
        return false;
    }

    if ($infoEmenda->limiteLiberado())
    {
        return false;
    }

    if ($dadosPeriodoAtual[$formaEx]['periodoaberto'] == 'f')
    {
        return false;
    }

    // -- Registro ainda em cadastramento
    if (empty($params['einid']))
    {
        return true;
    }

    $responsabilidade = new Spoemendas_Model_UsuarioResponsabilidade();
    if ($responsabilidade->superUsuario())
    {
        return true;
    }

    if (!workflowPerfil($infoEmenda))
    {
        return false;
    }

    return true;
}

/**
 *
 * @param type $infoEmenda
 *
 * @return boolean
 */
function workflowPerfil ($infoEmenda)
{
    /**
     * Fluxo do TED
     */
    $perfis = pegaPerfilGeral($_SESSION['usucpf']);
    if (in_array(PFL_TED, $perfis) && $infoEmenda->emPreenchimento())
    {
        return true;
    }
    if (in_array(PFL_SECRETARIAS, $perfis) && $infoEmenda->aguardandoAnaliseSecre())
    {
        return true;
    }
    /**
     * Fluxo da UO Equipe Tecnica
     */
    if (in_array(PFL_REITOR, $perfis) && $infoEmenda->aguardandoAprovacaoReitor())
    {
        return true;
    }
    if ((in_array(PFL_UO_EQUIPE_TECNICA, $perfis)|| in_array(PFL_UO_EQUIPE_FINANCEIRA, $perfis)) && ($infoEmenda->aguardandoAjustesUo() || $infoEmenda->emPreenchimento()))
    {
        return true;
    }
    /**
     * Fluxo do FNDE (Parlamentar)
     */
    if (in_array(PFL_ASPAR, $perfis) && $infoEmenda->analiseAspar())
    {
        return false;
    }
    if (in_array(PFL_PARLAMENTAR, $perfis) && $infoEmenda->emPreenchimento() || $infoEmenda->aguardandoAjustes())
    {
        return true;
    }
    if (in_array(PFL_FNDE, $perfis) && $infoEmenda->analiseFnde())
    {
        return false;
    }
}

/**
 *
 * @param type $params
 *
 * @return boolean
 */
function FndePreenche ($params)
{
    $infoEmenda = new Spoemendas_Model_Emendainformacoes($params['einid']);
    $perfis = pegaPerfilGeral($_SESSION['usucpf']);
    if (in_array(PFL_FNDE, $perfis) && $infoEmenda->analiseFnde())
    {
        return true;
    }
}

/**
 * validacaoTransicaoEmenda(emeid)
 *
 * @param type $emeid
 */
function validacaoTransicaoEmenda ($emeid)
{
    global $db;

    // -- deve haver ao menos um beneficiario para a emenda - passo 1
    $emendaDetalheEntidade = new Spoemendas_Model_Emendadetalheentidade();
    if (!(bool) ($beneficiarios = $emendaDetalheEntidade->findBeneficiariosByEmeid($emeid)))
    {
        return 'Você deve cadastrar ao menos um beneficiário para poder prosseguir.';
    }

    // -- todos os beneficiários tem que ter um CPF responsável - passo 2
    foreach ($beneficiarios ? $beneficiarios : [] as $beneficiario)
    {
        if (empty($beneficiario['edecpfresp']))
        {
            return 'Você deve definir os responsáveis de todos os beneficiários cadastrados para poder prosseguir.';
        }
    }

    // -- cada entidade tem que ter ao menos uma iniciativa - passo 3
    $iniciativaDetalheEntidade = new Spoemendas_Model_Iniciativadetalheentidade();
    $beneficiarios = $iniciativaDetalheEntidade->findBeneficiariosByEmeid($emeid);
    foreach ($beneficiarios ? $beneficiarios : [] as $beneficiario)
    {
        if (empty($beneficiario['iniid']))
        {
            return 'Você deve definir ao menos uma iniciativa para cada um dos beneficiários cadastrados para poder prosseguir.';
        }
    }

    // -- Caso a emenda seja do PAR (1 == SElECT etoid FROM emenda.emenda WHERE emeid = {$dados['emeid']}), cada beneficiário deve ter ao menos 1 iniciativa vinculada.
    if (1 == $db->pegaUm("SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}"))
    {
        $dadosIniciativa = $db->carregar("SELECT epi.*
  FROM emenda.emendadetalheentidade ede
    INNER JOIN emenda.emendadetalhe emd USING(emdid) and emdstatus = 'A'
    LEFT JOIN emenda.emendapariniciativa epi USING(edeid)
  WHERE emd.emeid = {$emeid}");

        foreach ($dadosIniciativa ? $dadosIniciativa : [] as $iniciativa)
        {
            if (empty($iniciativa['ppsid']))
            {
                return 'Você deve definir ao menos uma iniciativa para cada um dos responsáveis cadastrados para poder prosseguir.';
            }
        }
    }

    return true;
}

function formatarUnicod ($unicod, $dados)
{
    return "{$unicod} - {$dados['unidsc']}";
}

function formatarImpositivo ($impositivo)
{
    $tipo = ('Sim' == $impositivo) ? 'success' : 'danger';

    return <<<HTML
<span class="label label-{$tipo}">{$impositivo}</span>
HTML;
}

function formatarGnd ($gndcod, $dados)
{
    return "{$gndcod} - {$dados['gnddsc']}";
}

function formatarFonte ($foncod, $dados)
{
    return "{$foncod} - {$dados['fondsc']}";
}

function formatarModalidade ($mapcod, $dados)
{
    return "{$mapcod} - {$dados['mapdsc']}";
}

/*
 * Retorna o período atual e seus parâmetros
 */

function retornaDadosPeriodoAtual ()
{
    global $db;

    $perfis = pegaPerfilGeral($_SESSION['usucpf']);
    $perfisLinha = implode('\' OR perfiseditaveis = \'', $perfis);

    $sql = <<<DML
SELECT CASE WHEN CURRENT_DATE BETWEEN prf.prfdatainicio AND prf.prfdatafim
              THEN true
            ELSE false
         END AS periodoaberto,
       prf.prfid,
       prf.prfnome,
       to_char(prf.prfdatainicio, 'dd/mm/yyyy') AS prfdatainicio,
       to_char(prf.prfdatafim, 'dd/mm/yyyy') AS prfdatafim,
       prf.prfatual,
       prf.origenseditaveis,
       prf.perfiseditaveis,
       to_char(prfdatainicio, 'YYYY') as anoperiodo,
       abasvisiveis,
       regexp_replace (regexp_replace (
                          regexp_replace (prfmensagem, '(\\\\[_DATA_INICIAL_\\\\])+',
                                          TO_CHAR (prfdatainicio, 'DD/MM/YYYY')), '(\\\\[_DATA_FINAL_\\\\])+',
                          TO_CHAR (prfdatafim, 'DD/MM/YYYY')), '(\\\\[_DIAS_RESTANTES_\\\\])+',
                      date_part ('day', prfdatafim :: TIMESTAMP - prfdatainicio :: TIMESTAMP) || '') as prfmensagem
  FROM spoemendas.periodoreferencia prf
  WHERE prfatual = TRUE
    AND prf.prfano = '{$_SESSION['exercicio']}'
    AND ( perfiseditaveis = '{$perfisLinha}' ) -- consegue pesquisar apenas por perfil único
DML;

    $dados = $db->carregar($sql);
    $retorno = [];
    foreach (is_array($dados) ? $dados : [] as $linha)
    {
        $origens = explode(',', $linha['origenseditaveis']);
        foreach ($origens as $orig)
        {
            $retorno[$orig] = [
                'periodoaberto'    => $linha['periodoaberto'],
                'prfdatainicio'    => $linha['prfdatainicio'],
                'prfdatafim'       => $linha['prfdatafim'],
                'prfnome'          => $linha['prfnome'],
                'anoperiodo'       => $linha['anoperiodo'],
                'abasvisiveis'     => $linha['abasvisiveis'],
                'origenseditaveis' => $linha['origenseditaveis'],
                'perfiseditaveis'  => $linha['perfiseditaveis'],
                'prfmensagem'      => $linha['prfmensagem'],
            ];
        }
    }

    return $retorno;
}

function formatarImpeditivo ($ediimpositivo)
{
    switch ($ediimpositivo)
    {
        case 'TO':
            return '<span class="label label-danger">Total</span>';
        case 'NH':
            return '<span class="label label-success">Não há</span>';
        case 'PA':
            return '<span class="label label-warning">Parcial</span>';
    }
}

function formatarTipoSolicitacao ($tipo)
{
    switch ($tipo)
    {
        case 'A':
            return '<span class="label label-danger">Alteração</span>';
        case 'I':
            return '<span class="label label-success">Informação</span>';
    }
}

function formatarEmendasListagemSolicitacao ($emenda, $dados)
{
    if (isset($emenda))
    {
        $emendas = explode('/', $emenda);
        $emeano = $emendas[0];
        $emecod = $emendas[1];
        $emeid = $dados['emeid'];
        $saida = "<p style=\"text-align: center !important;\">"
//				 ."<a href= \"spoemendas.php?modulo=principal/emendas/listaremendas&acao=A&requisicao=filtrar&emendas[emecod]={$emecod}&emendas[exercicios]={$emeano}\" target=\"_blank\" >{$emenda}</a><br/>"
            . "<a href= \"spoemendas.php?modulo=principal/emendas/editaremenda/editaremenda&acao=A&emeid={$emeid}&aba=anexos\" target=\"_blank\" >{$emenda}</a><br/>"
            . "</p>";

        return $saida;
    }

    return "Solicitação sem anexo";
}

function camposObrigatorioUOEquipe ($emeid)
{
    global $db;
    $sqlDocid = "SELECT docid FROM spoemendas.emendainformacoes WHERE emeid = {$emeid}";
    $docid = $db->pegaUm($sqlDocid);
    $esdid = wf_pegarEstadoAtual($docid)['esdid'];
    $spoModel = new Spoemendas_Model_Emendaimpedimentobeneficiario();
    $query = $spoModel->pegaImpedimentos(['emeid' => $emeid], true);
    $result = $db->carregar($query);
    $msg = '';

    if (empty($result))
    {
        return "Deve ser preenchido um Plano de Trabalho ou Impedimento";
    }

    foreach ($result as $ptaImpedimento)
    {
        switch ($ptaImpedimento['ediimpositivo'])
        {
            case 'NH':
                if (empty($ptaImpedimento['edidescricao']))
                {
                    $msg .= 'Os campos devem ser preenchidos na aba Plano de Trabalho / Impedimento';
                }
                break;
            case 'PA' && 'TO':
                if (empty($ptaImpedimento['emiid']) || empty($ptaImpedimento['emiid']) || empty($ptaImpedimento['ireid'])
                )
                {
                    $msg .= 'Os campos devem ser preenchidos na aba Plano de Trabalho / Impedimento';
                }
                break;
            default:
                return "Deve ser preenchido um Plano de Trabalho ou Impedimento";
        }
    }
    if (empty($msg) && $esdid == ESD_ANALISA_SECRETARIA || $esdid == ESD_EM_ANALISE)
    {
        $EmendaImpedimento = condEmendaNaoImpedimento($emeid);

        return (!$EmendaImpedimento) ? false : true;
    }

    return (empty($msg) ? true : $msg);
}

function camposObrigParlamentarResposanveis ($emeid)
{
    return true;
    global $db;
    $sql = <<<DML
SELECT DISTINCT ede.edeid,
                enb.enbid,
                enb.enbcnpj,
                enb.enbnome,
                ede.edecpfresp,
                ede.edenomerep,
                ede.ededddresp,
                ede.edetelresp,
                edemailresp,
                TO_CHAR(enbdataalteracao, 'DD/MM/YYYY') AS data_alteracao
  FROM emenda.emendadetalhe emd
    JOIN emenda.emendadetalheentidade ede USING(emdid)
    JOIN emenda.entidadebeneficiada enb USING(enbid)
  WHERE emeid = {$emeid}
    AND enb.enbcnpj <> ''
    and emdstatus = 'A'
    AND enbstatus = 'A'
    AND edestatus = 'A'
DML;
    $result = $db->carregar($sql);
    $msgbool = false;
    if ($result)
    {
        foreach ($result as $value)
        {
            if (!$msgbool)
            {
                if (empty($value['edecpfresp']) || empty($value['edenomerep']) || empty($value['edenomerep']) ||
                    empty($value['edetelresp']) || empty($value['edemailresp'])
                )
                {
                    $msg .= "Preencher Passo 1 - Responsaveis pela elaboracao do Plano de Trabalho";
                    $msgbool = true;
                }
            }
        }
    }
    $sqlIndicacao = "SELECT DISTINCT
                edeid,
                enb.enbcnpj,
                enb.enbnome,
                emd.gndcod,
                emd.mapcod,
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
                WHERE emeid = {$emeid}
                     AND enb.enbcnpj <> ''
                     and emdstatus = 'A'
                      AND enbstatus = 'A'
                     AND edestatus = 'A'";
    $resultIndicacao = $db->carregar($sqlIndicacao);
    $indBoolen = false;
    if ($resultIndicacao)
    {
        foreach ($resultIndicacao as $ind)
        {
            $dados = detalharIniciativaBeneficiarioObrig($ind['edeid']);            
            $unidOrc = retornaDadosEmenda($emeid);
            
            if(!empty($unidOrc)){
                $unicod = explode('-', $unidOrc['unidade']);
                $unidadeOrc = (int) $unicod[0];
                // Se não for 26298 (FNDE) (Finalizar preenchimento sem preencher passo 2).
                if($unidadeOrc != 26298){
                    $indBoolen = true;
                }
                // Se for 26298 (FNDE) e for CONVENIO então Precisa preencher passo 2.
                if($unidadeOrc == 26298 && $unidOrc['etoid'] == EMENDA_DO_CONV){
                    $indBoolen = false;
                }
                // Se for 26298 (FNDE) e for PAR então não preenche Passo 2 (As informações devem vir do PAR).
                if($unidadeOrc == 26298 && $unidOrc['etoid'] == EMENDA_DO_PAR){
                    $indBoolen = true;
                }
            }
            
            if (!$indBoolen)
            {
                if (!$dados)
                {
                    $msg .= "<br>Preencher Passo 2 - Indicação de Iniciativas";
                    $indBoolen = true;
                }
            }
        }
    }

    $sql = "SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";
    $tipoEmenda = $db->pegaUm($sql);
    if ($tipoEmenda == EMENDA_DO_PAR || $tipoEmenda == EXECUCAO_PAR_CONV)
    {
        $sqlSubacao = "SELECT DISTINCT
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
                WHERE emeid = {$emeid}
                AND enb.enbcnpj <> ''
                AND enbstatus = 'A'
                and emdstatus = 'A'
                AND edestatus = 'A'
            ";
        $resultSubacao = $db->carregar($sqlSubacao);
        $subBoolean = false;
        if ($resultSubacao)
        {
            foreach ($resultSubacao as $sub)
            {
                $sub = detalharSubacoesPAR($sub['indicadas']);
                if (!$subBoolean)
                {
                    if (empty(trim($sub)))
                    {
                        if ($tipoEmenda == EMENDA_DO_PAR)
                        {
                            $msg .= "<br>Preencher Passo 3 - Subações do PAR";
                        }
                        else
                        {
                            $msg .= "<br>Preencher Passo 3 - Subações do PAR (Secretarias Estaduais e Prefeituras)";
                        }
                        $subBoolean = true;
                    }
                }
            }
        }
    }

    return (empty($msg)) ? true : $msg;
}

function detalharIniciativaBeneficiarioObrig ($edeid)
{
    global $db;

    $sql = "SELECT DISTINCT
                ininome,
                gndcod
            FROM
                emenda.iniciativadetalheentidade
            JOIN
                emenda.iniciativa
            USING
                (iniid)
            WHERE
                idestatus = 'A'
            AND edeid = {$edeid}
            ORDER BY
                1";
    $dados = $db->carregar($sql);

    return $dados;
}

function salvarAprovacaoReitor ($aprovacao, $emeid)
{
    global $db;
    if ($emeid)
    {
        $sql = "UPDATE emenda.emenda SET emeaprovacaoreitor = {$aprovacao} WHERE emeid = {$emeid} ";
        $db->executar($sql);
        $db->commit();
    }
}

function buscaAprovacaoReitor ($emeid)
{
    global $db;
    $sql = "SELECT emeaprovacaoreitor FROM emenda.emenda WHERE emeid = {$emeid} ";
    $result = $db->pegaUmObjeto($sql);

    return $result;
}

//function condicaoAprovaReitor($emeid)
//{
//	$result = buscaAprovacaoReitor($emeid);
//	if ($result->emeaprovacaoreitor == 't') {
//		return true;
//	} else {
//		return "Para continuar deve aprovar Plano de Trabalho / Impedimentos.";
//	}
//}

/**
 * Função que apaga todas as subações de uma emenda, que ainda não foram vinculadas no PAR
 *
 * @param      $edeid
 * @param      $anoexercicio
 * @param bool $autocommit - Utilizado caso de controle externo do commit
 *
 * @return bool|resource
 */
function apagaSubacoesNaoVinculadas ($edeid, $anoexercicio = null, $autocommit = true)
{
    global $db;
    $anoexercicio = empty($anoexercicio) ? $_SESSION['exercicio'] : $anoexercicio;
    $sql = <<<SQL
        DELETE FROM emenda.emendapariniciativa
        WHERE edeid = {$edeid}
        AND ppsid NOT IN
                  (
                      SELECT DISTINCT
                              s.ppsid
                      FROM par.subacao s
                      INNER JOIN par.subacaodetalhe sd ON sd.sbaid = s.sbaid
                      INNER JOIN par.subacaoemendapta sep ON sep.sbdid = sd.sbdid AND sep.sepstatus = 'A'
                      INNER JOIN emenda.v_emendadetalheentidade vede ON vede.emdid = sep.emdid AND vede.edestatus = 'A'
                      WHERE sd.sbdano = '{$anoexercicio}'
                      AND edeid = {$edeid}
                  )
SQL;
    $retorno = $db->executar($sql);
    if ($autocommit)
    {
        return $db->commit();
    }

    return $retorno;
}

function CondRetornarParaFnde ($emeid)
{
    global $db;
    $sql = "SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";
    $tipoEmenda = $db->pegaUm($sql);
    if ($tipoEmenda == EMENDA_DO_TED || $tipoEmenda == EMENDA_DO_DIR)
    {
        return false;
    }
    else
    {
        return true;
    }
}

function CondRetornarParaSecre ($emeid)
{
    global $db;
    $sql = "SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";
    $tipoEmenda = $db->pegaUm($sql);
    if ($tipoEmenda == EMENDA_DO_PAR || $tipoEmenda == EMENDA_DO_CONV || $tipoEmenda == EXECUCAO_PAR_CONV || $tipoEmenda == EMENDA_DO_TED)
    {
        return false;
    }
    else
    {
        return true;
    }
}

function CondRetornarParaAjustesUo ($emeid)
{
    global $db;
    $sql = "SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";
    $tipoEmenda = $db->pegaUm($sql);
    if ($tipoEmenda == EMENDA_DO_TED)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
 * Condição para retornar quando estiver no estado
 * "Aguardando Liberação de Limite"
 *
 * @global type $db
 *
 * @param type  $emeid
 *
 * @return boolean
 */
function CondRetornarTed ($emeid)
{
    global $db;
    $sql = "SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";
    $tipoEmenda = $db->pegaUm($sql);
    if ($tipoEmenda == EMENDA_DO_PAR || $tipoEmenda == EMENDA_DO_CONV || $tipoEmenda == EXECUCAO_PAR_CONV)
    {
        return false;
    }
    else
    {
        return true;
    }
}

function CondRetornarParaPreechimento ($emeid)
{
    global $db;
    $sql = "SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";
    $tipoEmenda = $db->pegaUm($sql);
    if ($tipoEmenda == EMENDA_DO_DIR)
    {
        return false;
    }
    else
    {
        return true;
    }
}

function recuperaFormaExecucao ($emeid)
{
    global $db;
    $sql = "SELECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";

    return $db->pegaUm($sql);
}

/**
 *
 * @global type $db
 *
 * @param type  $emeid
 *
 * @return boolean
 * Condição do workflow para verificar se existir beneficiario com impedimento,
 * se existir exibi a ação para enviar para "Emenda impedida"
 * e oculta "Aprovar e enviar para liberação".
 */
function condEmendaImpedimento ($emeid)
{
    global $db;
    $spoEmenda = new Spoemendas_Model_Emendaimpedimentobeneficiario();
    $query = $spoEmenda->pegaImpedimentos(['emeid' => $emeid], true);
    $result = $db->carregar($query);
    //Faz uma busca que pode haver mais de um beneficiario


    if(is_array($result)){
	    foreach ($result as $value)
	    {
	        if ($value['ediimpositivo'])
	        {
	            $cond[] = ($value['ediimpositivo'] != 'NH') ? 'impedimento' : 'NH';
	        }
	    }
    }
    if ($cond)
    {
        $existeImpedimento = array_search('impedimento', $cond);
    }
    if ($cond[$existeImpedimento] == 'impedimento')
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 *
 * @global type $db
 *
 * @param type  $emeid
 *
 * @return boolean
 * Condição do workflow para verificar se existir beneficiario com impedimento,
 * se existir exibi a ação para enviar para "Aprovar e enviar para liberação"
 * e oculta "Emenda impedida".
 */
function condEmendaNaoImpedimento ($emeid)
{
    global $db;
    $spoEmenda = new Spoemendas_Model_Emendaimpedimentobeneficiario();
    $query = $spoEmenda->pegaImpedimentos(['emeid' => $emeid], true);
    $result = $db->carregar($query);
    //Faz uma busca que pode haver mais de um beneficiario
    foreach ($result as $value)
    {
        if ($value['ediimpositivo'])
        {
            $cond[] = ($value['ediimpositivo'] == 'NH') ? 'NH' : 'impedimento';
        }
    }
    if ($cond)
    {
        $existeImpedimento = array_search('impedimento', $cond);
    }
    if ($cond[$existeImpedimento] == 'NH')
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 *
 * @global type $db
 *
 * @param type  $emeid
 *
 * @return boolean
 * Condição para quando estiver no estado "Emenda com impedimento" retornar para
 * o estado "Aguardando elaboração do plano de trabalho - FNDE"
 */
function condRetornarImpedimentoFnde ($emeid)
{
    global $db;
    $sql = "SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";
    $result = $db->pegaUm($sql);
    $emendas = [
        EMENDA_DO_TED,
        EMENDA_DO_DIR
    ];
    if (!in_array($result, $emendas))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 *
 * @global type $db
 *
 * @param type  $emeid
 *
 * @return boolean
 * Condição do workflow quando estiver no estado "Emenda com impedimento" retornar
 * para o estado "Aguardando analise da secretaria"
 */
function condRetornarImpedimentoUo ($emeid)
{
    global $db;
    $sql = "SElECT etoid FROM emenda.emenda WHERE emeid = {$emeid}";
    $result = $db->pegaUm($sql);
    $emendas = [
        EMENDA_DO_PAR,
        EMENDA_DO_CONV,
        EXECUCAO_PAR_CONV
    ];
    if (!in_array($result, $emendas))
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * Formata o código da UO, para que apareça a sigla e no tooltip o nome da UO
 *
 * @param string $unidade Código do PI para formatação.
 */
function formatarCodigoUnidade ($unicod, $dados)
{
    if (empty($unicod))
    {
        return '-';
    }

    return <<<HTML
<abbr data-toggle="tooltip" data-placement="top"  title="{$dados['unidsc']}">{$unicod} - {$dados['uniabrev']}</abbr>
HTML;
}

function formatarVazio ($campo)
{
    return empty($campo) ? '-' : $campo;
}

function formatarUfMunicipio ($uf, $dados)
{
    return "{$uf} - {$dados['mundescricao']}";
}

function workflowSolicitacao ($docid, $sfnid)
{
    if (!empty($docid))
    {
        include_once APPRAIZ . 'includes/workflow.php';
        echo wf_desenhaBarraNavegacaoBootstrap($docid, ['sfnid' => $sfnid]);
    }
}


function pegaSituacaoAtual ($sfnid)
{
    $sql = <<<DML
        SELECT
            d.esdid
        FROM spoemendas.solicitacaofinanceira sf
        JOIN workflow.documento d ON (d.docid = sf.docid)
        WHERE sf.sfnid = {$sfnid}
DML;
    global $db;

    return $db->pegaUm($sql);
}


function pegaimpedimentos2 ($edivalor)
{
    if ($edivalor === null)
    {
        return 'Não ha';
    }
    else
    {
        return mascaraMoeda($edivalor);
    }
}

function aprovarEnvioAnalise ()
{
    if ($_POST)
    {
        if ($_POST['checkbox'] == 1)
        {
            $retorno = ['boo' => true, 'msg' => ''];
        }
        else
        {
            $retorno = ['boo' => false, 'msg' => 'Operação não pôde ser realizada.'];
        }

        $retorno = simec_json_encode($retorno);

        echo $retorno;
    }
}

function form_aprovarEnvioAnalise ()
{
    $html = <<<HTML
	<form action="" method="post" name="form">
		<input type="checkbox" name="checkbox" id="capeta" value="1"> Aprovar programa de trabalho ou impedimento da emenda
	</form>

	<script>
	$(document).ready(function(){
	    $('.modal-footer > .btn-primary').addClass("disabled");

		$('#capeta').change(function() {
		   if($("#capeta").is(':checked')){
		       $('.modal-footer > .btn-primary').removeClass("disabled");
		   }else{
		       $('.modal-footer > .btn-primary').addClass("disabled");
		   }
		});
	});
	</script>

HTML;


    echo $html;
}

/**
 * Função que extrai os dados do excel
 *
 * @param $arquivo_temp
 *
 * @return mixed
 */
function extrairDadosCSV ($arquivo_temp, $idCarga, $arqid)
{
    global $db;

    $modelCargaTesouro = new Spoemendas_Model_Cargatesourogerencial();

    $insert = <<<SQL
          INSERT INTO spoemendas.cargatesourogerencial (
    unicod,unidsc, orgcod, orgdsc, ungcod, ungdsc, gestaocod,
    ungcodprincipal, uniabrev, ctgresulteof, ctgdesceof,
    ptres, gndcod, gnddsc, emeano, ne, ctgfontesof, ctgfonterecursodetalhada,
    ctgdescfonterecurso, regcod, descricaouf, mundsc, ctgnreferencia, ctgsisorigem,
    ctgconvsiafi, ctgdespempenhadas, ctgdespliquidas, ctgdespliquidadaspagar,
    ctgdesppagas, ctgrapprocesspagar, ctgrapnprocessliquipagar, ctgrapprocnproc,
    ctgpagprocnproc, ctgrappagprocnproc, ctgvalliquidopagar,  ctgpo, usucpf, ctgidentcarga,
    ctgprocessado, ctgimportado, arqid
) VALUES
SQL;

    $values = [];
    $AllValues = [];

    $validacao = $modelCargaTesouro->camposValidacao();
    $cabecalho = retornoCabecalhoExcel();

    $cont = 0;
    try
    {
        $separador = ';';
        $f = fopen($arquivo_temp, "r");
        $line = fgetcsv($f, null, $separador);

        //Verifica a quantidade de colunas de acordo com o modelo
        if (count($line) != 36)
        {
            fclose($f);
            $f = fopen($arquivo_temp, "r");
            $separador = ',';
            $line = fgetcsv($f, null, $separador);

            if (count($line) != 36)
            {
                return NAO_MODELO;
            }
        }

        while ($linha = fgetcsv($f, null, $separador))
        {
            if (verificaArquivoVazio([$AllValues]) == 'vazio')
            {
                return 'vazio';
            }

            if (!tratarCamposNumerico($linha))
            {
                return CONTEM_LETRAS_VALOR;
            }
//		    ver($linha, $validacao, $cabecalho, d);
            $msg = $modelCargaTesouro->validaTamanhoCampos([$linha], $validacao, $cabecalho);
            if ($msg != EXCEL_TAMANHO_CAMPOS_CORRETO)
            {
                return $msg;
            }

            foreach ($linha as $key => &$item)
            {
                if ($key >= 25)
                {
                    break;
                }

                if (is_string($item))
                {
                    $item = pg_escape_string(($item));
                }
            }

            $values[] = "('{$linha[0]}', '{$linha[1]}','{$linha[2]}','{$linha[3]}','{$linha[4]}','{$linha[5]}',
                          '{$linha[6]}', '{$linha[7]}','{$linha[8]}','{$linha[9]}','{$linha[10]}','{$linha[12]}',
                          '{$linha[13]}','{$linha[14]}','{$linha[15]}','{$linha[16]}', '{$linha[17]}','{$linha[18]}',
                          '{$linha[19]}','{$linha[20]}','{$linha[21]}','{$linha[22]}','{$linha[23]}','{$linha[24]}',
                           {$linha[25]},{$linha[26]},{$linha[27]},{$linha[28]},{$linha[29]},{$linha[30]},
                           {$linha[31]},{$linha[32]},{$linha[33]},{$linha[34]},{$linha[35]},'{$linha[11]}',
                          '{$_SESSION['usucpf']}', {$idCarga}, 'FALSE', 'TRUE', {$arqid})";

            $AllValues = $linha;

            $cont++;
        }
    } finally
    {
        fclose($f);
    }


    $db->executar($insert . implode(',', $values));
    $db->commit();

    //Arquivo não possui nem um tipo de dados no Excel
    if ($AllValues == false)
    {
        return $AllValues;
    }

    //Verifica se a planilha esta vazia contendo cabecalho
    return $cont;

}

/**
 * Função verifica se o arquivo possui dados
 *
 * @param array $AllValues Array
 *
 * @return String
 */
function verificaArquivoVazio ($AllValues)
{
    $vazio = 0;
    for ($j = 0; $j <= 0; $j++)
    {
        for ($i = 0; $i <= sizeof($AllValues[$j]) - 1; $i++)
        {
            if (empty($AllValues[$j][$i]))
            {
                $vazio++;
            }
        }
    }

    return ($vazio == 36) ? 'vazio' : 'possui';

}

function pegarIdentificadorCarga ()
{
    global $db;

    $idCarga = $db->pegaUm("SELECT MAX(ctgidentcarga) FROM spoemendas.cargatesourogerencial");
    if (!$idCarga)
    {
        $idCarga = 1;
    }
    else
    {
        $idCarga++;
    }

    return $idCarga;
}


/**
 * Apresenta caso o valor seja menor ou maior.
 *
 * @param type $valor
 *
 * @return type String
 */
function apresentacaoDiferencaValores ($valor)
{
    $valores = explode(' | ', $valor);
    $val = mascaraMoeda($valores[0], null, true);
    $dif = mascaraMoeda($valores[1], null, true);
    if ($valores[1] > 0)
    {
        $diff = <<<HTMLUP
            <div class="text-left">{$val}</div>
            <div class="text-right pull-right" style="color:blue;width: 100%"><span style="color: green" class="pull-left glyphicon glyphicon-chevron-up"></span>{$dif}</div>
HTMLUP;
    }
    elseif ($valores[1] < 0)
    {
        $diff = <<<HTMLDOWN
            <div class="text-left">{$val}</div>
            <div class="text-right pull-right" style='color:red;width: 100%'><span class='pull-left glyphicon glyphicon-chevron-down'></span>{$dif}</div>
HTMLDOWN;

    }
    else
    {
        $diff = $val;
    }

    return $diff;
}

/**
 * Formata o nome da unidade adicionado "Variação".
 *
 * @param type $nome String
 *
 * @return type String
 */
function formataNomeUnidade ($nome)
{
    $newName = <<<HTML
            <div class="text-left">{$nome}</div>
            <div class="text-right pull-left" style="width: 100%">Variação</div>
HTML;

    return $newName;
}

/**
 * Função para verificar se os campos com valor possuir letras.
 *
 * @param array $vals Array
 *
 * @return boolean
 */
function tratarCamposNumerico (&$vals)
{
    for ($i = 26; $i < count($vals); $i++)
    {
        if (!empty($vals[$i]))
        {
            $vals[$i] = removeMascaraMoeda($vals[$i]);

            if (strpos($vals[$i], '(') !== false)
            {
                $vals[$i] = (str_replace(['(', ')'], '', $vals[$i])) * (-1);
            }


            $valsTemp = trim($vals[$i]);
            if (!is_numeric($valsTemp) && !is_null($vals[$i]) && $valsTemp != '')
            {
                return false;
            }

        }
        else
        {
            $vals[$i] = 0;
        }
    }

    $vals[6] = str_pad($vals[6], 5, '0', STR_PAD_LEFT);
    $vals[12] = str_pad($vals[12], 6, '0', STR_PAD_LEFT);
    $vals[17] = str_pad($vals[17], 4, '0', STR_PAD_LEFT);
    $vals[18] = str_pad($vals[18], 10, '0', STR_PAD_LEFT);
    $vals[25] = intval($vals[25]);

    return true;
}

/**
 * Gera as colunas em letras para apresentar excel
 *
 * @return string
 */
function retornoCabecalhoExcel ()
{
    for ($i = 'A'; $i != 'Y'; ++$i)
    {
        $cols[] = $i;
    }

    return $cols;
}


/**
 * Verifica os dados de preenchimento da solicitação.
 *
 * @param mixed[] $dados Dados recuperados do banco ou do $_POST.
 *
 * @return boolean|string True ou mensagem de erro.
 */
function condValidaPreenchimento ($dados, $converteMoeda = true)
{
    $mensagem = [];
    if (!is_array($dados) || empty($dados))
    {
        $mensagem[] = "Os dados informados são inválidos";
    }
    if (empty($dados['sfninteressado']))
    {
        if (strpos($dados['autnome'], 'Relator Geral') !== false
            || strpos($dados['autnome'], 'Com.') !== false
            || strpos($dados['autnome'], 'Bancada') !== false)
        {
            $mensagem[] = 'O campo Interessado é de preenchimento obrigatório';
        }
    }
    if (empty($dados['sfnugexecutora']))
    {
        $mensagem[] = 'O campo UG Executora é de preenchimento obrigatório';
    }
    if (empty($dados['sfnnotaempenho']))
    {
        $mensagem[] = 'O campo Nota de Empenho é de preenchimento obrigatório';
    }
    if (empty($dados['sfncodvinculacao']))
    {
        $mensagem[] = 'O campo Vinculação é de preenchimento obrigatório';
    }
    if (empty($dados['sfnfontedetalhada']))
    {
        $mensagem[] = 'O campo Fonte de Recurso Detalhada é de preenchimento obrigatório';
    }
    if (empty($dados['estuf']))
    {
        $mensagem[] = 'O campo UF é de preenchimento obrigatório';
    }
    if (empty($dados['muncod']) || $dados['muncod'] == 'SEM INFORMACAO')
    {
        $mensagem[] = 'O campo Municipio é de preenchimento obrigatório';
    }

    $numeroReferencia = ($dados['sfnnumeroreferencia'] == 'O')
        ? $dados['sfnnumeroreferenciaoutros']
        : $dados['sfnnumeroreferencia'];

    if ((empty($dados['sfncontratorepasse']))
        && (empty($dados['sfnpropostasiconv']))
        && (empty($dados['sfnconveniosiafi']))
        && (empty($numeroReferencia))
    )
    {
        $mensagem[] = 'Ao menos um dos quatro campos deve ser preenchidos: Contrato Repasse, Proposta no SICONV, Convênio SIAFI ou Nº de Referência constante na nota de empenho no SIAFI';
    }

    if (empty($dados['sfnmequipamento']))
    {
        $mensagem[] = 'O campo de Máquina ou Equipamento é de preenchimento obrigatório';
    }
    if (empty($dados['sfnobjetivo']))
    {
        $mensagem[] = 'O campo Objeto é de preenchimento obrigatório';
    }
    if (empty($dados['sfngrauprioridade']))
    {
        $mensagem[] = 'O campo de Grau de prioridade é de preenchimento obrigatório';
    }

    $valorSolicitacao = $dados['sfpvalorpedido'];
    $valorLimiteSolicitacao = empty($dados['sfpvalorpedido_hidden']) ? '0' : $dados['sfpvalorpedido_hidden'];

    $modSolFinanceira = new Spoemendas_Model_Solicitacaofinanceira($dados['sfnid']);
    if($dados['sfnnotaempenho'] && !$valorLimiteSolicitacao) {
        $valorLimiteSolicitacao = $modSolFinanceira->inputValorSolicitar($dados['sfnid'], $dados['sfnnotaempenho']);
    }

    if (empty($valorLimiteSolicitacao) || $valorLimiteSolicitacao == '0.00')
    {
        $mensagem[] = 'A NE digitada não possui limite de valor a solicitar, portanto, não há possibilidade de solicitar um valor';
    }
    if ($valorSolicitacao > $valorLimiteSolicitacao)
    {
        $mensagem[] = 'O valor solicitado é maior que o limite de solicitação (R$ ' . mascaraMoeda($valorLimiteSolicitacao, false,true) . ')';
    }
    if (empty($valorSolicitacao))
    {
        $mensagem[] = 'O valor solicitado não pode ser zero ou vazio';
    }
    if (empty($mensagem))
    {
        return true;
    }

    $msg = "<h4 class=\'text-left\'>Antes de prosseguir, verifique o(s) seguinte(s) erro(s):</h4><br>";
    foreach ($mensagem as $m) {
        $msg.= "<p class=\'text-left\'>- {$m}</p>";
    }

    return $msg;
}

/**
 * Formata o status do anexo p/ determinados estados do workflow.
 *
 * @param string $status - Estado do workflow
 */
function formatarStatusAnexo ($status)
{
    $formata = true;

    switch ($status)
    {
        case 'Aguardando prazo de alteração' :
            $tooltip = 'O pedido foi recebido e está aguardando abertura do SIOP/MP para enviarmos a solicitação.';
            break;
        case 'Aguardando publicação' :
            $tooltip = 'O pedido foi cadastrado no SIOP/MP. Aguardando efetivação e/ou publicação no DOU.';
            break;
        case 'Devolvido para ASPAR' :
            $tooltip = 'Pedido devolvido para ASPAR para solicitação de informações complementares por parte do Parlamentar.';
            break;
        default :
            $formata = false;
            break;
    }

    if ($formata)
    {
        return <<<HTML
<abbr data-toggle="tooltip" data-placement="top"  title="{$tooltip}">{$status}</abbr>
HTML;
    }

    return $status;
}

/**
 * Formata código da emenda em link para abrir modal com detalhes da emenda. (Listagem Extrato dinâmico)
 *
 * OBS: Formata somente quando Exercício esta na presente na query
 *
 * @param string $value - Emecod (Código da emenda)
 * @param array  $row   - Linha da consulta
 */
function formatarEmecodEmLink ($value, $row)
{
    global $db;

    if (isset($row['emeano']))
    {
        $sql = "SELECT emeid FROM emenda.emenda WHERE emecod = '{$value}' AND emeano = {$row['emeano']}";

        $value = <<<HTML
	<a href="javascript:;" onclick="verEmenda({$db->pegaUm($sql)})">{$value}</a>
HTML;
    }

    return $value;
}

function pegarIdentificadorUpload ($table, $column)
{
    global $db;

    $idCarga = $db->pegaUm("SELECT MAX(" . $column . ") FROM spoemendas." . $table);
    if (!$idCarga)
    {
        $idCarga = 1;
    }
    else
    {
        $idCarga++;
    }

    return $idCarga;
}

/**
 * Função para verificar se os campos com valor possuir letras.
 *
 * @param array $vals   string
 * @param array $coluna string
 * @param array $linha  int
 *
 * @return boolean
 */
function tratarCamposNumericoSegov (&$vals, $delimitador)
{
    if (trim($vals) !== '')
    {
        if(';' == $delimitador){
            $vals = trim(removeMascaraMoeda($vals));
        }else{
            $vals = trim($vals);
        }
        $valsTemp = trim($vals);
        if (!is_numeric($valsTemp) && !is_null($vals) && $valsTemp != '')
        {
            return false;
        }
    }
    else
    {
        return 'vazio';
    }

    return $vals;
}

/**
 * Valida formato da data
 * Valida data
 *
 * @param $date
 *
 * @return bool
 */
function validaData ($data)
{
    if (strlen($data))
    {
        $datePart = explode('/', $data);
        if (count($datePart) === 3)
        {
            return (checkdate($datePart[1], $datePart[0], $datePart[2])) ? true : false;
        }
    }

    return false;
}


function combinarArrays ($arr1, $arr2, $arr3)
{
    $arr1 = !is_array($arr1) ? [] : $arr1;
    $arr2 = !is_array($arr2) ? [] : $arr2;
    $arr3 = !is_array($arr3) ? [] : $arr3;

    $arrayFull = array_merge(array_merge($arr1, $arr2), $arr3);
    $dados = [$arr1, $arr2, $arr3];

    $saida = [];
    foreach ($arrayFull as $key => $item)
    {
        $saida[$key] = validacaoMultipla($key, $dados);
    }

    return $saida;
}

function validacaoMultipla($key, $arrays)
{
    foreach ($arrays as $array){
        if (validaCampoArray($array, $key)!== false)
            return $array[$key];
    }
}

function validaCampoArray ($arr, $key)
{
    if (key_exists($key, $arr))
    {
        if (!empty($arr[$key]))
        {
            return $arr[$key];
        }
    }

    return false;
}
/**
 * Método responsável por formatar os dados para combo no estilo Simec_Form
 *
 * @author Victor Eduardo Barreto
 * @param array  $dados Dados a serem tratados
 * @param string $id    Identificador do dado
 * @param string $dsc   Descrição do dado
 * @return array Dados tratados para combo
 */
function formatarCombo ($dados, $id, $dsc)
{
    $ret = [];
    if (!empty($dados)) {
        foreach ($dados as $dado) {
            $ret[$dado[$id]] = $dado[$dsc];
        }
    }
    return $ret;
}

/**
 * Método que determina qual início será apresentado, de acordo com o perfil do usuário logado.
 *
 * @param $perfis
 */
function apresentaInicio($perfis) {
    if (in_array(PFL_PARLAMENTAR, $perfis)) {
        require_once APPRAIZ . 'spoemendas/modulos/inicioParlamentar.inc';
    } else {
            require_once APPRAIZ . 'spoemendas/modulos/inicioDefault.inc';
    }
}


// ---------- Funções do Dashboard do perfil SPO / Equipe Financeira ----------

/**
 * Método que apresenta o calendário da página inicial do perfil SPO/Equipe Financiera
 *
 */
function apresentaCalendarioEquipeFinanceira($mes = null){
    $mes = empty($mes) ? date('m') : $mes;

    $calendario = (new Simec_Calendario($_SESSION['exercicio'], $mes))
        ->mostrarQuantosMesesAntes($mes-1)
        ->mostrarQuantosMesesDepois(12-$mes)
        ->addScript(
            <<<SCRIPT
                if (this.id) {
                    var to = $('#' + this.id).data('to');
                    $('#mescorrente').val(to.month);
                    carregaGraficoMensalEf(to.month);
                    carregaGraficoMensal(to.month);
                    carregaGraficoPizza(to.month);
                }
SCRIPT
        )
        ->setLegenda(
            [
                [
                    'type'      => 'text',
                    'label'     => 'Hoje',
                    'badge'     => '00',
                    'classname' => 'badge-today'
                ]
            ]
        );

    $periodos = (new Spoemendas_Model_Periodosolicitacao)->retornaPeriodosDoAno($_SESSION['exercicio']);

    if (!empty($periodos)) {
        $cores = [
            'rgba(87, 140, 169, 0.4)',
            'rgba(246, 209, 85, 0.4)',
            'rgba(0, 75, 141, 0.4)',
            'rgba(242, 85, 44, 0.4)',
            'rgba(149, 222, 227, 0.4)',
            'rgba(237, 205, 194, 0.4)',
            'rgba(206, 49, 117, 0.4)',
            'rgba(90, 114, 71, 0.4)',
            'rgba(207, 176, 149, 0.4)',
            'rgba(76, 106, 146, 0.4)',
            'rgba(146, 182, 213, 0.4)',
            'rgba(71, 90, 114, 0.4)',
        ];

        foreach ($periodos as $index => $periodo) {
            $calendario
                ->addEventoPeriodo(
                    str_replace('/', '-', $periodo['prsdatainicio']),
                    str_replace('/', '-', $periodo['prsdatafim']),
                    false,
                    false,
                    'Periodo de Solicitação Aberto em ' . $periodo['prsdescricao'],
                    false,
                    $cores[$index],
                    false
                )
                ->addLegenda(
                    'block',
                    mascaraMesCurtoAnoTexto($periodo['prsmes']),
                    null,
                    $cores[$index]
                );
        }
    }

    $calendario->render();
}

function apresentaGrafico($div, $prsmes = null, $titulo = null, $nodata = null, $posfixo = null) {
    $nodata = empty($nodata) ? 'Nenhum registro encontrado' : $nodata;
    $posfixoInicial = !empty($prsmes) ? 'Mes' : 'Ano';;
    $posfixo = !empty($posfixo) ? ' - '.$posfixo : '';
    $titulo .= $posfixo;


    $modelSolicitacao = new Spoemendas_Model_Solicitacaofinanceira();
    $dadosGrupoUnidade = $modelSolicitacao->dadosGraficoSolicitacaoMensalGupoUnidade($_SESSION['exercicio'], $prsmes);
    $dadosUnidade = $modelSolicitacao->dadosGraficoSolicitacaoMensalUnidade($_SESSION['exercicio'], $prsmes);
    $dadosUnidadeGestora = $modelSolicitacao->dadosGraficoSolicitacaoMensalUnidadeGestora($_SESSION['exercicio'], $prsmes);
    $dadosEmpenho = $modelSolicitacao->dadosGraficoSolicitacaoMensalEmpenho($_SESSION['exercicio'], $prsmes);
    $dadosEmenda = $modelSolicitacao->dadosGraficoSolicitacaoMensalEmenda($_SESSION['exercicio'], $prsmes);

    $categorias = [];
    $valorsolicitado = [];
    $valorlimite = [];
    $valorautorizado = [];
    $drilldown = [];
    $ddegeral = [];
    $ddemgeral = [];
    $ddemggeral = [];
    $i = 0;
    foreach ($dadosGrupoUnidade as $dado) {
        $drildownID = $dado['gundsc'];
        $categorias[] = $dado['gundsc'];

        $valorlimite[] = ['name' => $drildownID, 'title' => 'Solicitações por Unidade'.$posfixo, 'y' => $dado['valorlimite'], 'drilldown' => $drildownID.'-valorlimite'];
        $valorsolicitado[] = ['name' => $drildownID, 'title' => 'Solicitações por Unidade'.$posfixo, 'y' => $dado['valorsolicitado'], 'drilldown' => $drildownID.'-valorsolicitado'];
        $valorautorizado[] = ['name' => $drildownID, 'title' => 'Solicitações por Unidade'.$posfixo, 'y' => $dado['valorautorizado'], 'drilldown' => $drildownID.'-valorautorizado'];


        // -- Drilldown das unidades
        $ddvalorlimite = [];
        $ddvalorsolicitado = [];
        $ddvalorautorizado = [];

        $ddvalorlimite[$i]['id'] = $drildownID.'-valorlimite';
        $ddvalorlimite[$i]['name'] = 'Valor Limite (R$)';

        $ddvalorsolicitado[$i]['id'] = $drildownID.'-valorsolicitado';
        $ddvalorsolicitado[$i]['name'] = 'Valor Solicitado (R$)';

        $ddvalorautorizado[$i]['id'] = $drildownID.'-valorautorizado';
        $ddvalorautorizado[$i]['name'] = 'Valor Autorizado (R$)';
        foreach ($dadosUnidade as $item) {
            if ($item['gundsc'] == $dado['gundsc']) {
                if (!empty($item['uniabrev'])) {
                    $ddvalorlimite[$i]['data'][] = ['name' => $item['uniabrev'], 'title' => 'Solicitações por Unidade Gestora' . $posfixo, 'y' => $item['valorlimite'], 'drilldown' => $item['uniabrev'] . '-valorlimite'];
                    $ddvalorsolicitado[$i]['data'][] = ['name' => $item['uniabrev'], 'title' => 'Solicitações por Unidade Gestora' . $posfixo, 'y' => $item['valorsolicitado'], 'drilldown' => $item['uniabrev'] . '-valorsolicitado'];
                    $ddvalorautorizado[$i]['data'][] = ['name' => $item['uniabrev'], 'title' => 'Solicitações por Unidade Gestora' . $posfixo, 'y' => $item['valorautorizado'], 'drilldown' => $item['uniabrev'] . '-valorautorizado'];
                }


                // -- Drilldown das emendas
                $ddevalorlimite = [];
                $ddevalorsolicitado = [];
                $ddevalorautorizado = [];

                $ddevalorlimite[$i]['id'] = $item['uniabrev'] . '-valorlimite';
                $ddevalorlimite[$i]['name'] = 'Valor Limite (R$)';

                $ddevalorsolicitado[$i]['id'] = $item['uniabrev'] . '-valorsolicitado';
                $ddevalorsolicitado[$i]['name'] = 'Valor Solicitado (R$)';

                $ddevalorautorizado[$i]['id'] = $item['uniabrev'] . '-valorautorizado';
                $ddevalorautorizado[$i]['name'] = 'Valor Autorizado (R$)';

                foreach ($dadosUnidadeGestora as $itemUG) {
                    if ($item['uniabrev'] == $itemUG['uniabrev']) {
                        $ddevalorlimite[$i]['data'][] = ['name' => $itemUG['ungdsc'], 'title' => 'Solicitações por Emenda' . $posfixo, 'y' => $itemUG['valorlimite'], 'drilldown' => $itemUG['ungdsc'] . '-valorlimite'];
                        $ddevalorsolicitado[$i]['data'][] = ['name' => $itemUG['ungdsc'], 'title' => 'Solicitações por Emenda' . $posfixo, 'y' => $itemUG['valorsolicitado'], 'drilldown' => $itemUG['ungdsc'] . '-valorsolicitado'];
                        $ddevalorautorizado[$i]['data'][] = ['name' => $itemUG['ungdsc'], 'title' => 'Solicitações por Emenda' . $posfixo, 'y' => $itemUG['valorautorizado'], 'drilldown' => $itemUG['ungdsc'] . '-valorautorizado'];


                        // -- Drilldown das emendas
                        $ddemgvalorlimite = [];
                        $ddemgvalorsolicitado = [];
                        $ddemgvalorautorizado = [];

                        $ddemgvalorlimite[$i]['id'] = $itemUG['ungdsc'] . '-valorlimite';
                        $ddemgvalorlimite[$i]['name'] = 'Valor Limite (R$)';

                        $ddemgvalorsolicitado[$i]['id'] = $itemUG['ungdsc'] . '-valorsolicitado';
                        $ddemgvalorsolicitado[$i]['name'] = 'Valor Solicitado (R$)';

                        $ddemgvalorautorizado[$i]['id'] = $itemUG['ungdsc'] . '-valorautorizado';
                        $ddemgvalorautorizado[$i]['name'] = 'Valor Autorizado (R$)';

                        foreach ($dadosEmenda as $item2) {
                            if ($itemUG['ungcod'] == $item2['ungcod']) {
                                if (!empty($item2['emecod'])) {
                                    $ddemgvalorlimite[$i]['data'][] = ['name' => $item2['emecod'], 'title' => 'Solicitações por Nota de Empenho' . $posfixo, 'y' => $item2['valorlimite'], 'drilldown' => $item2['emecod'] . '-valorlimite'];
                                    $ddemgvalorsolicitado[$i]['data'][] = ['name' => $item2['emecod'], 'title' => 'Solicitações por Nota de Empenho' . $posfixo, 'y' => $item2['valorsolicitado'], 'drilldown' => $item2['emecod'] . '-valorsolicitado'];
                                    $ddemgvalorautorizado[$i]['data'][] = ['name' => $item2['emecod'], 'title' => 'Solicitações por Nota de Empenho' . $posfixo, 'y' => $item2['valorautorizado'], 'drilldown' => $item2['emecod'] . '-valorautorizado'];
                                }


                                // -- Drilldown das empenho
                                $ddemvalorlimite = [];
                                $ddemvalorsolicitado = [];
                                $ddemvalorautorizado = [];

                                $ddemvalorlimite[$i]['id'] = $item2['emecod'] . '-valorlimite';
                                $ddemvalorlimite[$i]['name'] = 'Valor Limite (R$)';

                                $ddemvalorsolicitado[$i]['id'] = $item2['emecod'] . '-valorsolicitado';
                                $ddemvalorsolicitado[$i]['name'] = 'Valor Solicitado (R$)';

                                $ddemvalorautorizado[$i]['id'] = $item2['emecod'] . '-valorautorizado';
                                $ddemvalorautorizado[$i]['name'] = 'Valor Autorizado (R$)';
                                foreach ($dadosEmpenho as $item3) {
                                    if ($item2['emecod'] == $item3['emecod']) {
                                        if (!empty($item3['emecod'])) {
                                            $ddemvalorlimite[$i]['data'][] = ['name' => $item3['sfnnotaempenho'], 'y' => $item3['valorlimite']];
                                            $ddemvalorsolicitado[$i]['data'][] = ['name' => $item3['sfnnotaempenho'], 'y' => $item3['valorsolicitado']];
                                            $ddemvalorautorizado[$i]['data'][] = ['name' => $item3['sfnnotaempenho'], 'y' => $item3['valorautorizado']];
                                        }
                                    }
                                }
                                $ddemgeral = array_merge(
                                    $ddemgeral,
                                    $ddemvalorlimite,
                                    $ddemvalorsolicitado,
                                    $ddemvalorautorizado
                                );
                            }
                        }

                        $ddemggeral = array_merge(
                            $ddemggeral,
                            $ddemgvalorlimite,
                            $ddemgvalorsolicitado,
                            $ddemgvalorautorizado
                        );
                    }
                }

                $ddegeral = array_merge(
                    $ddegeral,
                    $ddevalorlimite,
                    $ddevalorsolicitado,
                    $ddevalorautorizado
                );
            }

        }
        $drilldown = array_merge(
            $drilldown,
            $ddvalorlimite,
            $ddvalorsolicitado,
            $ddvalorautorizado,
            $ddegeral,
            $ddemggeral,
            $ddemgeral
        );

        $i++;
    }

    $categorias = simec_json_encode($categorias);
    $valorlimite = simec_json_encode($valorlimite, JSON_NUMERIC_CHECK);
    $valorsolicitado = simec_json_encode($valorsolicitado, JSON_NUMERIC_CHECK);
    $valorautorizado = simec_json_encode($valorautorizado, JSON_NUMERIC_CHECK);
    $drilldown = simec_json_encode($drilldown, JSON_NUMERIC_CHECK);
//    categories: {$categorias},
    $js =  <<<HTML
    <script>
        Highcharts.setOptions({
            lang: {
                decimalPoint: ',',
                thousandsSep: '.',
                drillUpText: 'Voltar',
                noData: '{$nodata}'
            }
        });
        var title = [];
        // var posfixo = '{$posfixo}';
        var titulo = '{$titulo}';
        var chart{$posfixoInicial} = new Highcharts.chart('{$div}', {
          chart: {
              id:'{$posfixo}',
            type: 'column',
            height: '366px',
              events: {
                drilldown: function(e) {
                    var chart = this;
                     console.info(title);
                    title[chart.drilldownLevels ? chart.drilldownLevels.length : 0] = chart.title.textStr;
                    chart.setTitle({
                        text: e.point.title
                    });
                },
                drillup: function(e) {
                    var chart = this;
                    console.info(title);
                    title[chart.drilldownLevels ? chart.drilldownLevels.length : 0] = chart.title.textStr;
                    chart.setTitle({
                        text: title[chart.drilldownLevels.length]
                    });
                }
              }
          },
          title: {
            text: titulo
          },
          xAxis: {
            type: 'category',
            labels: {
              rotation: -45
            }
          },
          yAxis: {
            min: 0,
            title: {
              text: 'Valor (em R$)',
              align: 'high'
            },
            labels: {
              overflow: 'justify',
              rotation: -45
            }
          },
          tooltip: {
            valuePrefix: 'R$ ',
            shared: true
          },
          plotOptions: {
            column: {
              dataLabels: {
                enabled: true,
                format: 'R$ {point.y:,.2f}'
              }
            }
          },
          legend: {
            backgroundColor: ((Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'),
          },
          credits: {
            enabled: false
          },
          exporting: {
            enabled: false
          },
          series: [
            {
                name: 'Valor Limite (R$)',
                 data: {$valorlimite}
            },
            {
                name: 'Valor Solicitado (R$)',
                 data: {$valorsolicitado}
            },
            {
                name: 'Valor Autorizado (R$)',
                 data: {$valorautorizado}
            }
          ],
          drilldown: {
              drillUpButton: {
                  position: {
                      x: 0,
                      y:-50
                  }
              },
              series: {$drilldown}
          }
        });
    </script>
HTML;

    echo $js;
}

function apresentaGraficoMensal($div, $prsmes = null, $titulo = null, $nodata = null, $posfixo = null) {
    $nodata = empty($nodata) ? 'Nenhum registro encontrado' : $nodata;
    $posfixo = !empty($posfixo) ? ' - '.$posfixo : '';

    $modelSolicitacao = new Spoemendas_Model_Solicitacaofinanceira();
    $dadosMeses = $modelSolicitacao->dadosGraficoSolicitacaoMes($_SESSION['exercicio']);
    $dadosEmpenho = $modelSolicitacao->dadosGraficoSolicitacaoMensalEmpenho($_SESSION['exercicio']);

    $categorias = [];
    $valorsolicitado = [];
    $valorlimite = [];
    $valorautorizado = [];
    $drilldown = [];
    $ddegeral = [];
    $ddemgeral = [];
    $i = 0;
    foreach ($dadosMeses as $dado) {
        $drildownID = trim($dado['mesdsc']);
        $categorias[] = $dado['mesdsc'];

        $valorlimite[] = ['name' => $drildownID, 'title' => 'Solicitações'.$posfixo, 'y' => $dado['valorlimite'], 'drilldown' => $drildownID.'-valorlimite'];
        $valorsolicitado[] = ['name' => $drildownID, 'title' => 'Solicitações'.$posfixo, 'y' => $dado['valorsolicitado'], 'drilldown' => $drildownID.'-valorsolicitado'];
        $valorautorizado[] = ['name' => $drildownID, 'title' => 'Solicitações'.$posfixo, 'y' => $dado['valorautorizado'], 'drilldown' => $drildownID.'-valorautorizado'];

        // -- Drilldown das empenho
        $ddemvalorlimite = [];
        $ddemvalorsolicitado = [];
        $ddemvalorautorizado = [];

        $ddemvalorlimite[$i]['id'] = $drildownID.'-valorlimite';
        $ddemvalorlimite[$i]['name'] = 'Valor Limite (R$)';

        $ddemvalorsolicitado[$i]['id'] = $drildownID.'-valorsolicitado';
        $ddemvalorsolicitado[$i]['name'] = 'Valor Solicitado (R$)';

        $ddemvalorautorizado[$i]['id'] = $drildownID.'-valorautorizado';
        $ddemvalorautorizado[$i]['name'] = 'Valor Autorizado (R$)';
        foreach ($dadosEmpenho as $item3) {
            if (trim($dado['mesdsc']) == trim($item3['mesdsc'])) {
                $ddemvalorlimite[$i]['data'][] = ['name' => $item3['sfnnotaempenho'], 'y' => $item3['valorlimite']];
                $ddemvalorsolicitado[$i]['data'][] = ['name' => $item3['sfnnotaempenho'], 'y' => $item3['valorsolicitado']];
                $ddemvalorautorizado[$i]['data'][] = ['name' => $item3['sfnnotaempenho'], 'y' => $item3['valorautorizado']];
            }
        }
        $drilldown = array_merge(
            $drilldown,
            $ddemvalorlimite,
            $ddemvalorsolicitado,
            $ddemvalorautorizado
        );


        $i++;
    }

    $categorias = simec_json_encode($categorias);
    $valorlimite = simec_json_encode($valorlimite, JSON_NUMERIC_CHECK);
    $valorsolicitado = simec_json_encode($valorsolicitado, JSON_NUMERIC_CHECK);
    $valorautorizado = simec_json_encode($valorautorizado, JSON_NUMERIC_CHECK);
    $drilldown = simec_json_encode($drilldown, JSON_NUMERIC_CHECK);
//    categories: {$categorias},
    $js =  <<<HTML
    <script>
        Highcharts.setOptions({
            lang: {
                decimalPoint: ',',
                thousandsSep: '.',
                drillUpText: 'Voltar',
                noData: '{$nodata}'
            }
        });
        var title = [];
        var posfixo = '{$posfixo}';
        var titulo = '{$titulo}';
        Highcharts.chart('{$div}', {
          chart: {
            type: 'column',
            height: '500px',
              events: {
                drilldown: function(e) {
                    var chart = this;
                    title[chart.drilldownLevels ? chart.drilldownLevels.length : 0] = chart.title.textStr;
                    chart.setTitle({
                        text: e.point.title
                    });
                },
                drillup: function(e) {
                    var chart = this;
                    chart.setTitle({
                        text: title[chart.drilldownLevels.length]
                    });
                }
              }
          },
          title: {
            text: titulo + posfixo
          },
          xAxis: {
            type: 'category',
            labels: {
              rotation: -45
            }
          },
          yAxis: {
            min: 0,
            title: {
              text: 'Valor (em R$)',
              align: 'high'
            },
            labels: {
              overflow: 'justify',
              rotation: -45
            }
          },
          tooltip: {
            valuePrefix: 'R$ ',
            shared: true
          },
          plotOptions: {
            column: {
              dataLabels: {
                enabled: true,
                format: 'R$ {point.y:,.2f}'
              }
            }
          },
          legend: {
            backgroundColor: ((Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'),
          },
          credits: {
            enabled: false
          },
          exporting: {
            enabled: false
          },
          series: [
            {
                name: 'Valor Limite (R$)',
                 data: {$valorlimite}
            },
            {
                name: 'Valor Solicitado (R$)',
                 data: {$valorsolicitado}
            },
            {
                name: 'Valor Autorizado (R$)',
                 data: {$valorautorizado}
            }
          ],
          drilldown: {
              drillUpButton: {
                  position: {
                      x: 0,
                      y:-50
                  }
              },
              series: {$drilldown}
          }
        });
    </script>
HTML;

    echo $js;
}

function apresentaGraficoEmpenhoMensal($div, $prsmes = null, $titulo = null, $nodata = null, $posfixo = null) {
    $nodata = empty($nodata) ? 'Nenhum registro encontrado' : $nodata;
    $posfixo = !empty($posfixo) ? ' - '.$posfixo : '';

    $modelSolicitacao = new Spoemendas_Model_Solicitacaofinanceira();
    $dadosEmpenho = $modelSolicitacao->dadosGraficoSolicitacaoMensalEmpenho($_SESSION['exercicio'], $prsmes);

    $categorias = [];
    $valorsolicitado = [];
    $valorlimite = [];
    $valorautorizado = [];
    $drilldown = [];
    $ddegeral = [];
    $ddemgeral = [];
    $i = 0;
    foreach ($dadosEmpenho as $dado) {
        $drildownID = trim($dado['sfnnotaempenho']);
        $categorias[] = $dado['sfnnotaempenho'];

        $valorlimite[] = ['name' => $drildownID, 'title' => 'Solicitações Mensais', 'y' => $dado['valorlimite']];
        $valorsolicitado[] = ['name' => $drildownID, 'title' => 'Solicitações Mensais', 'y' => $dado['valorsolicitado']];
        $valorautorizado[] = ['name' => $drildownID, 'title' => 'Solicitações Mensais', 'y' => $dado['valorautorizado']];

        $i++;
    }

    $categorias = simec_json_encode($categorias);
    $valorlimite = simec_json_encode($valorlimite, JSON_NUMERIC_CHECK);
    $valorsolicitado = simec_json_encode($valorsolicitado, JSON_NUMERIC_CHECK);
    $valorautorizado = simec_json_encode($valorautorizado, JSON_NUMERIC_CHECK);
//    categories: {$categorias},
    $js =  <<<HTML
    <script>
        Highcharts.setOptions({
            lang: {
                decimalPoint: ',',
                thousandsSep: '.',
                drillUpText: 'Voltar',
                noData: '{$nodata}'
            }
        });
        var title = [];
        var posfixo = '{$posfixo}';
        var titulo = '{$titulo}'
        Highcharts.chart('{$div}', {
          chart: {
            type: 'column',
            height: '500px',
          },
          title: {
            text: titulo + posfixo
          },
          xAxis: {
            type: 'category',
            labels: {
              rotation: -45
            }
          },
          yAxis: {
            min: 0,
            title: {
              text: 'Valor (em R$)',
              align: 'high'
            },
            labels: {
              overflow: 'justify',
              rotation: -45
            }
          },
          tooltip: {
            valuePrefix: 'R$ ',
            shared: true
          },
          plotOptions: {
            column: {
              dataLabels: {
                enabled: true,
                format: 'R$ {point.y:,.2f}'
              }
            }
          },
          legend: {
            backgroundColor: ((Highcharts.theme && Highcharts.theme.legendBackgroundColor) || '#FFFFFF'),
          },
          credits: {
            enabled: false
          },
          exporting: {
            enabled: false
          },
          series: [
            {
                name: 'Valor Limite (R$)',
                 data: {$valorlimite}
            },
            {
                name: 'Valor Solicitado (R$)',
                 data: {$valorsolicitado}
            },
            {
                name: 'Valor Autorizado (R$)',
                 data: {$valorautorizado}
            }
          ]
        });
    </script>
HTML;

    echo $js;
}

function apresentaGraficoPedidoPorSituacao($prsmes) {
    global $db;

    $strSQL = <<<DML
SELECT
  count(sfn.sfnid)
FROM spoemendas.solicitacaofinanceira sfn
  INNER JOIN spoemendas.solicitacaofinanceirapedido sfp ON (sfn.sfnid = sfp.sfnid AND sfp.sfpstatus = 'A')
  INNER JOIN spoemendas.periodosolicitacao prs ON (prs.prsid = sfp.prsid AND prs.prsmes = {$prsmes})
  RIGHT JOIN spoemendas.solicitacaofinanceirasituacao sfs USING(sfsid)
GROUP BY sfsid
ORDER BY sfsid;
DML;

    $sql = <<<DML
SELECT
    count(sfp.sfpid)
FROM spoemendas.solicitacaofinanceirapedido sfp
INNER JOIN spoemendas.solicitacaofinanceira sfn ON (sfn.sfnid = sfp.sfnid)
INNER JOIN spoemendas.solicitacaofinanceirasituacao sfs USING(sfsid)
DML;

    $total = $db->pegaUm($sql);

    $resultado = $db->carregar($strSQL);

    $pendente = $resultado[0]['count'];
    $autorizado = $resultado[1]['count'];
    $parcial = $resultado[2]['count'];
    $naoautorizado = $resultado[3]['count'];
    $naosolicitado = $resultado[4]['count'];

    $js =  <<<JSCRIPT
<script type="text/javascript">
 Highcharts.chart('div-grafico-pizza', {
    chart: {
        plotBackgroundColor: null,
        plotBorderWidth: null,
        plotShadow: false,
        type: 'pie'
    },
    title: {
        text: 'Situação dos Pedidos'
    },
    tooltip: {
        pointFormat: '{series.name}: <b>{point.y} ({point.percentage:.1f} %)</b>'
    },
    plotOptions: {
        pie: {
            allowPointSelect: true,
            cursor: 'pointer',
            size: '75%',
            dataLabels: {
                enabled: true,
                format: '<b>{point.name}</b>: {point.y} ({point.percentage:.1f} %)',
                style: {
                    color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black'
                }
            }
        }
    },
    credits : {
        enabled: false
    },
    series: [{
        name: 'Solicitações',
        colorByPoint: true,
        data: [{
            name: 'Não Solicitados',
            id: 5,
            events: {
                click: function(e) {
                    openListaSolilcitacoes(this.id, $('#mescorrente').val());
                }
            },
            y: {$naosolicitado}
        }, {
            name: 'Autorizados',
            id: 2,
            events: {
                click: function(e) {
                    openListaSolilcitacoes(this.id, $('#mescorrente').val());
                }
            },
            y: {$autorizado},
            sliced: true,
            selected: true
        }, {
            name: 'Autorizado Parcialmente',
            id: 3,
            events: {
                click: function(e) {
                    openListaSolilcitacoes(this.id, $('#mescorrente').val());
                }
            },
            y: {$parcial}
        }, {
            name: 'Não Autorizados',
            id: 4,
            events: {
                click: function(e) {
                    openListaSolilcitacoes(this.id, $('#mescorrente').val());
                }
            },
            y: {$naoautorizado}
        }, {
            name: 'Pendente de Autorização',
            id: 1,
            events: {
                click: function(e) {
                    openListaSolilcitacoes(this.id, $('#mescorrente').val());
                }
            },
            y: {$pendente}
        }]
    }]
});
</script>
JSCRIPT;

    echo $js;
}

/**
 * @return array
 */
function retornaCpfExtratoDinamicoFinanceiro(){
    global $db;

    $clausulas = [];

    if (in_array(PFL_UO_EQUIPE_FINANCEIRA, pegaPerfilGeral())) {
        $pflcod = PFL_UO_EQUIPE_FINANCEIRA;

        $sql = <<<SQL
SELECT DISTINCT usucpf
FROM public.extratodinamicoconsultas t1
    JOIN seguranca.usuario u USING (usucpf)
    JOIN seguranca.usuario_sistema us USING (usucpf)
    JOIN seguranca.sistema s USING (sisid)
    JOIN (seguranca.perfilusuario
        JOIN seguranca.perfil USING (pflcod)
         ) p USING (usucpf, sisid)
WHERE
    p.pflstatus = 'A'
    AND us.susstatus = 'A'
    AND s.sisstatus = 'A'
    AND s.sisdiretorio = 'spoemendas'
    AND p.pflcod = '{$pflcod}'
SQL;
        $retorno = $db->carregar($sql);

        $retorno = !is_array($retorno) ? [] : $retorno;

        $retorno = array_map(
            function ($dado) {
                return "'{$dado['usucpf']}'";
            },
            $retorno
        );
        $clausulas[] = 't1.usucpf IN (' . implode(', ', $retorno) . ')';
    }

    return $clausulas;
}

/**
 * @return array
 */
function retornaUnicodExtratoDinamicoFinanceiro(){
    global $db;

    $clausulas = [];

    if (in_array(PFL_UO_EQUIPE_FINANCEIRA, pegaPerfilGeral())) {
        $pflcod = PFL_UO_EQUIPE_FINANCEIRA;

        $sql = <<<SQL
SELECT DISTINCT unicod
FROM spoemendas.usuarioresponsabilidade
WHERE usucpf = '{$_SESSION['usucpf']}'
SQL;
        $retorno = $db->carregar($sql);

        $retorno = !is_array($retorno) ? [] : $retorno;

        $retorno = array_map(
            function ($dado) {
                return "'{$dado['unicod']}'";
            },
            $retorno
        );
        $clausulas[] = 'unicod IN (' . implode(', ', $retorno) . ')';
    }

    return $clausulas;
}

/**
 * @param $criterioUnicod
 *
 * @return string
 */
function retornaJSExtratoDinamicoFinanceiro($criterioUnicod){
    if (in_array(PFL_UO_EQUIPE_FINANCEIRA, pegaPerfilGeral())) {
        $criterioUnicod = implode(' AND ', $criterioUnicod);

        global $db;

        $sql = <<<SQL
SELECT DISTINCT
      u.unicod                      AS codigo
    , u.unicod || ' - ' || u.unidsc AS descricao
FROM public.unidade u
WHERE {$criterioUnicod}
ORDER BY
    2
SQL;

        $retorno = $db->carregar($sql);

        $json = simec_json_encode($retorno ? $retorno : []);
        $jsonSelecionados = simec_json_encode($_POST['financeiro']['unicod'] ? $_POST['financeiro']['unicod'] : []);

        $js = <<<JS
        $().ready(function () {
            // trata Unidade
            var unicod = $('#financeiro_unicod');

            unicod.find('option').remove();

            unicodSelecionados = {$jsonSelecionados};

            $.each({$json}, function (index, el) {
                unicod.append($('<option>', {
                        value: el.codigo,
                        text: el.descricao,
                        selected: $.inArray(el.codigo, unicodSelecionados) != -1
                    })
                );
            });

            unicod.trigger('chosen:updated');

            // trata salvar somente privado
            $("#financeiro_formarmazenar_edcpublico").closest('.form-group').remove()
        });
JS;
    }

    return $js;
}