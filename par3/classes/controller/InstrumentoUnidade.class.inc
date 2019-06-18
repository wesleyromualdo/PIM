<?php

error_reporting(0);

/**
 * Par_Controle_Instrumentounidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 23-09-2015
 * @link     no link
 */
class Par3_Controller_InstrumentoUnidade extends Controle
{


    public function __construct()
    {
        parent::__construct();
    }

    /**
     * Função lista
     * - monta a lista de instrumentos unidade.
     *
     * @return escreve a lista.
     *
     */
    public function lista($arrPost)
    {

        $perfis = pegaPerfilGeral($_SESSION['usucpf']);

        $sql = Par3_Model_InstrumentoUnidade::pegarSQLLista($arrPost);

        $cabecalho = self::cabecalhoListaPrincipal($arrPost);
        $listagem = new Simec_Listagem();

        $arrColunasEscondidas = array();
        switch($arrPost['alternar']){
            case 'planejamento':
                $arrColunasEscondidas[] = 'percentual';
                array_push($cabecalho, 'Planejamento');
                break;
            case 'diagnostico':
                $arrColunasEscondidas[] = 'planejamento';
                array_push($cabecalho, '% Diagnóstico');
                break;
            case 'todos':
                array_push($cabecalho, '% Diagnóstico');
                array_push($cabecalho, 'Planejamento');
                break;
            default:
                $arrColunasEscondidas[] = 'planejamento';
                array_push($cabecalho, '% Diagnóstico');
                break;
        }


        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        if(in_array(PAR3_PERFIL_SUPER_USUARIO_DESENVOLVEDOR, $perfis) || in_array(PAR3_PERFIL_SUPER_USUARIO, $perfis)){
            $listagem->addAcao('retornar', ['func' => 'modalSituacao', 'titulo' => 'Voltar Situação']);
        }
        $listagem->addAcao('view', ['func' => 'dadosUnidade', 'titulo' => 'Visualizar']);

        $listagem->addCallbackDeCampo('percentual', 'colunaPercentualDignoticoListaMunicipios');
        $listagem->addCallbackDeCampo('planejamento', 'colunaPercentualDignoticoListaMunicipiosPlanejamento');

        $listagem->setId('listaMunicipiosTable');
        if ($arrPost['itrid'] != '2'){
            $arrColunasEscondidas[] = 'estuf';
            $listagem->esconderColuna($arrColunasEscondidas);
            $listagem->setId('listaEstadosTable');
        }
        $listagem->esconderColuna($arrColunasEscondidas);
        $listagem->esconderColunas(['itrid', 'inuid', 'esdid']);
        $listagem->setFormFiltros('formulario');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->turnOnOrdenacao('estuf');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }

    public function buscaEstadoMunicipioUnidade($inuid)
    {
        global $db;

        $sql = "
            SELECT  CASE WHEN itrid = 1
                        THEN estuf
                        ELSE muncod
                    END AS mun_est_cod
            FROM par3.instrumentounidade
            WHERE inuid = {$inuid}
        ";
        return $db->pegaUm($sql);
    }

    public function buscaEsferaUnidade($inuid)
    {
        global $db;

        $sql = "
            SELECT  CASE WHEN itrid = 1
                        THEN 'E'
                        ELSE 'M'
                    END AS esfera
            FROM par3.instrumentounidade
            WHERE inuid = {$inuid}
        ";
        return $db->pegaUm($sql);
    }
    
    public function pegarEscopoPendencias(){
        $model = new Par3_Model_InstrumentoUnidade();
        return $model->pegarEscopoPendencias();
    }

    public function pegarEscopoAlertas(){
        $model = new Par3_Model_InstrumentoUnidade();
        return $model->pegarEscopoAlertas();
    }

    
    /**
     * Thiago Mariano
     * - retorna o muncod .
     *
     * @return integer.
     *
     */
    public function pegarMuncod($inuid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        return $modelInstrumentoUnidade->muncod;

    }//end pegarMuncod();

    /**
     * Thiago Mariano
     * - retorna o municipio.
     *
     * @return integer.
     *
     */
    public function pegarMunicipio($inuid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        return $modelInstrumentoUnidade->inudescricao;

    }//end pegarMunicipio();

    /**
     * Função cabecalhoUnidade
     * - monta o cabeçalho da unidade.
     *
     * @return escreve a lista.
     *
     */
    public function pegarItrid($inuid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        return $modelInstrumentoUnidade->itrid;

    }//end pegarItrid();


    /**
     * Função pegarNomeEntidade
     * - Monta o nome da entidade.
     *
     * @return escreve o nome da entidade.
     *
     */
    public function pegarNomeEntidade($inuid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $entidade = $modelInstrumentoUnidade->inudescricao;
        $estuf = $modelInstrumentoUnidade->estuf;
        $itrid = $modelInstrumentoUnidade->itrid;

        return ($itrid == 1) ? $entidade : $entidade . ' - ' . $estuf;
    }
    
    
    /**
     * Função pegarPendenciasPrestacaoContas
     * - Monta as pendencias da Prestacao Contas.
     *
     * @return Lista de pendencias.
     *
     */
    public function pegarPendenciasPrestacaoContas($inuid)
    {
        global $db;
        
        $sql = "WITH wf_municipio_PAR3 AS
        (
           SELECT
              i.inuid as inuid_par,
              iu.inuid as inuid_par3
           FROM
              par.instrumentounidade i
              JOIN par3.instrumentounidade iu ON ((i.muncod = iu.muncod) AND (i.mun_estuf IS NOT NULL))
                 OR (((i.estuf = iu.estuf) AND (i.muncod IS NULL)) AND (iu.muncod IS NULL ))
        )
        select
           *
        from
           (
              SELECT DISTINCT
                prp.prpnumeroprocesso as processo,
                dp.dopnumerodocumento::text as documento,
                CASE WHEN to_date(dp.dopdatafimvigencia, 'MM/YYYY') > to_date('02/07/2018', 'DD/MM/YYYY')
				          THEN TO_CHAR(date_trunc('month', to_date(dp.dopdatafimvigencia, 'MM/YYYY'))+ '1 month' + '59 day':: INTERVAL , 'DD/MM/YYYY')
					        ELSE TO_CHAR(to_date('31/08/2018', 'DD/MM/YYYY') , 'DD/MM/YYYY')
				        END AS data_vigencia,
                'PAR' AS tipo,
                'Omissão no envio de prestação de contas' AS situacao
      FROM par.instrumentounidade inu
       INNER JOIN par.processopar prp ON inu.inuid = prp.inuid
                                           AND prp.prpstatus = 'A'
       INNER JOIN par.documentopar dp ON prp.prpid = dp.prpid
                                           AND dp.dopstatus = 'A'
       INNER JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
       INNER JOIN par.situacaoopc opc ON prp.prpid = opc.prpid
       INNER JOIN workflow.documento d ON opc.docid = d.docid
                                            AND d.esdid = 2298
       INNER JOIN wf_municipio_PAR3 mp3 ON inu.inuid = mp3.inuid_par
        WHERE
         mp3.inuid_par3 = {$inuid}
         AND prp.prpid NOT IN
         (
            SELECT
               efd.prpid
            FROM
               par.efeitosuspensivodocumento efd
            WHERE
               efd.prpid = prp.prpid
               AND efd.efdstatus = 'A'
         )
         UNION ALL
         SELECT DISTINCT
                    pro.pronumeroprocesso as processo,
                    dp.dopnumerodocumento::text as documento,
                    CASE WHEN to_date(dp.dopdatafimvigencia, 'MM/YYYY') > to_date('02/07/2018', 'DD/MM/YYYY')
    				          THEN TO_CHAR(date_trunc('month', to_date(dp.dopdatafimvigencia, 'MM/YYYY'))+ '1 month' + '59 day':: INTERVAL , 'DD/MM/YYYY')
    					        ELSE TO_CHAR(to_date('31/08/2018', 'DD/MM/YYYY') , 'DD/MM/YYYY')
    				        END AS data_vigencia,
                    'Obras PAR' AS tipo,
                    'Omissão no envio de prestação de contas' AS situacao
        FROM par.instrumentounidade inu
               INNER JOIN par.processoobraspar pro ON inu.inuid = pro.inuid
                                                        AND pro.prostatus = 'A'
               INNER JOIN par.documentopar dp ON pro.proid = dp.proid
                                                   AND dp.dopstatus = 'A'
               INNER JOIN par.termocomposicao pto ON pto.dopid = dp.dopid
               LEFT JOIN par.modelosdocumentos md ON md.mdoid = dp.mdoid
               INNER JOIN obras2.situacaoopcobras opc ON pro.proid = opc.proid_par
               INNER JOIN workflow.documento d ON opc.docid = d.docid
                                                    AND d.esdid = 2278
               INNER JOIN wf_municipio_PAR3 mp3 ON inu.inuid = mp3.inuid_par
        WHERE mp3.inuid_par3 = {$inuid}
          AND pro.proid NOT IN
              (SELECT efd.proid
               FROM par.efeitosuspensivodocumento efd
               WHERE efd.proid = pro.proid
                 AND efd.efdstatus = 'A')
        UNION ALL
        SELECT DISTINCT
                        pro.pronumeroprocesso as processo,
                        par.retornanumerotermopac(tc.proid)::text as documento,
                        CASE WHEN to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1), 'DD/MM/YYYY') > to_date('02/07/2018', 'DD/MM/YYYY')
                          THEN TO_CHAR(to_date((select to_char(tcp.terdatafimvigencia,'DD/MM/YYYY') from par.termocompromissopac tcp where tcp.proid = pro.proid AND terstatus = 'A' LIMIT 1),'DD/MM/YYYY') + INTERVAL '60 day', 'DD/MM/YYYY')
                          ELSE TO_CHAR(to_date('31/08/2018', 'DD/MM/YYYY'), 'DD/MM/YYYY')
                        END AS data_vigencia,
                        'PAC' AS tipo,
                        'Omissão no envio de prestação de contas' AS situacao
        FROM par.termocompromissopac tc
               INNER JOIN par.processoobra pro ON pro.proid = tc.proid
                                                    AND pro.prostatus = 'A'
               INNER JOIN par.termoobra ter ON tc.terid = ter.terid
               INNER JOIN obras.preobra po ON ter.preid = po.preid
               INNER JOIN par.instrumentounidade inu ON (tc.muncod = inu.muncod
                                                           OR tc.estuf = inu.estuf)
               INNER JOIN obras2.situacaoopcobras opc ON pro.proid = opc.proid_pac
               INNER JOIN workflow.documento d ON opc.docid = d.docid
                                                    AND d.esdid = 2278
               INNER JOIN wf_municipio_PAR3 mp3 ON inu.inuid = mp3.inuid_par
        WHERE mp3.inuid_par3 = {$inuid}
            AND tc.terstatus = 'A'
            AND pro.proid NOT IN
            (
            SELECT
            efd.proid_pac
            FROM
            par.efeitosuspensivodocumento efd
            WHERE
            efd.proid_pac = pro.proid
            AND efd.efdstatus = 'A'
            )
           ) as a";

        return $db->carregar($sql);
    }


    /**
     * Função cabecalhoListaPrincipal
     * - monta o cabeçalho da lista principal.
     *
     * @return cabeçalho.
     *
     */
    public function cabecalhoListaPrincipal($arrPost)
    {
        $cabecalho = array();

        if ($arrPost['itrid'] == '2') $cabecalho[] = 'UF';

        $cabecalho[] = ($arrPost['itrid'] == '2') ? 'Município' : 'Estado';

        $cabecalho[] = 'Situação';

        return $cabecalho;
    }//end cabecalhoListaPrincipal();


    /**
     * Função pegarDescricaoEsfera
     * - monta o cabeçalho da unidade.
     *
     * @return escreve a lista.
     *
     */
    public function pegarDescricaoEsfera($inuid)
    {
        $itrid = self::pegarItrid($inuid);
        return ($itrid === '2') ? 'Municipal' : 'Estadual';
    }//end pegarDescricaoEsfera();


    /**
     * Função pegarDescricaoEsfera
     * - monta o cabeçalho da unidade.
     *
     * @return escreve a lista.
     *
     */
    public function pegarDescricaoEsferaDif($inuid)
    {
        $itrid = self::pegarItrid($inuid);
        return ($itrid === '2') ? 'municipio' : 'estado';
    }//end pegarDescricaoEsfera();

    /**
     * Função cabecalhoUnidade
     * - monta o cabeçalho da unidade.
     *
     * @return escreve a lista.
     *
     */
    public function cabecalhoUnidade()
    {
        include APPRAIZ . 'par3/modulos/principal/planoTrabalho/cabecalhoUnidade.inc';

    }//end cabecalhoUnidade();

    /**
     * @param $inuid
     * @return float|int
     */
    public function verificaPercentualPreenchimentoUnidade($inuid)
    {
        $entidade = new Par3_Controller_InstrumentoUnidadeEntidade();
        $controleUnidade = new Par3_Controller_InstrumentoUnidade();
        $modelCACS = new Par3_Model_CACS();
        $controlCAE = new Par3_Controller_CAE();

        $itrid = $controleUnidade->pegarItrid($inuid);

        $percentual = [];
        array_push($percentual, $entidade->verificaPreencimentoEquipe($inuid));
        array_push($percentual, $entidade->verificaPreencimentoNutricionistas($inuid));
        array_push($percentual, $modelCACS->verificaPreenchimentoConselheiros($itrid, $inuid));
        array_push($percentual, $entidade->verificaPreencimentoConselho($inuid));
        array_push($percentual, $controlCAE->verificaPreenchimentoCAE($inuid));

        if ($itrid == '2') {
            array_push($percentual, $entidade->verificaPreencimentoDirigente($inuid));
            array_push($percentual, $entidade->verificaPreencimentoSecretaria($inuid));
        }

        return array_sum($percentual) / count($percentual);
    }


    /**
     * @param $inuid
     * @return array
     */
    public function verificaPreencimentoUnidadeDetalhado($inuid)
    {
        $entidade = new Par3_Controller_InstrumentoUnidadeEntidade();
        $controleUnidade = new Par3_Controller_InstrumentoUnidade();
        $modelCACS = new Par3_Model_CACS();
        $controlCAE = new Par3_Controller_CAE();

        $itrid = $controleUnidade->pegarItrid($inuid);

        $retorno = [];
        $urlBase = 'par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid=';

        if ($itrid == '2') {
            $esfera = $controleUnidade->pegarDescricaoEsfera($inuid);

            $preenchimentoSecretaria = $entidade->verificaPreencimentoSecretaria($inuid);
            $retorno[] = [
                'descricao' => "Secretaria {$esfera} de Educação",
                'percent' => $preenchimentoSecretaria,
                'total' => 1,
                'url' => $urlBase . $inuid . '&menu=secretaria',
            ];

            $preenchimentoDirigente = $entidade->verificaPreencimentoDirigente($inuid);
            $retorno[] = [
                'descricao' => "Dirigente {$esfera} de Educação",
                'percent' => $preenchimentoDirigente,
                'total' => 1,
                'url' => $urlBase . $inuid . '&menu=dirigente',
            ];
        }

        $preenchimentoEquipe = $entidade->verificaPreencimentoEquipe($inuid);
        $retorno[] = [
            'descricao' => 'Equipe Local',
            'percent' => $preenchimentoEquipe,
            'total' => 1,
            'url' => $urlBase . $inuid . '&menu=equipe',
        ];

        $preenchimentoNutricionista = $entidade->verificaPreencimentoNutricionistas($inuid);
        $retorno[] = [
            'descricao' => 'Equipe Nutricionistas',
            'percent' => $preenchimentoNutricionista,
            'total' => 1,
            'url' => $urlBase . $inuid . '&menu=nutricionista',
        ];

        $testaPreenchimentoComite = $modelCACS->verificaPreenchimentoConselheiros($itrid, $inuid);
        $retorno[] = [
            'descricao' => 'Conselho de Acompanhamento e Controle Social',
            'percent' => $testaPreenchimentoComite,
            'total' => 1,
            'url' => $urlBase . $inuid . '&menu=comite',
        ];

        $testaPreenchimentoConselho = $entidade->verificaPreencimentoConselho($inuid);
        $retorno[] = [
            'descricao' => 'Conselho Municipal de Educação',
            'percent' => $testaPreenchimentoConselho,
            'total' => 1,
            'url' => $urlBase . $inuid . '&menu=conselho',
        ];

        $testaPreenchimentoCAE = $controlCAE->verificaPreenchimentoCAE($inuid);
        $retorno[] = [
            'descricao' => 'Conselho de Alimentação Escolar',
            'percent' => $testaPreenchimentoCAE,
            'total' => 1,
            'url' => $urlBase . $inuid . '&menu=cae',
        ];

        return $retorno;
    }


    /**
     * Função verificaPreencimentoQuestoes
     * - verifica o preenchimento da aba dados da unidade e retorna um percentual
     * de preenchimento.
     *
     * @return escreve a lista.
     *
     */
    public function verificaPreencimentoQuestoes($inuid, $detalhado = false)
    {
        $modelUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $preenchimento = $modelUnidade->retornaPreenchimentoQuestoes($inuid);

        $percPNE = $this->verificaPreencimentoQuestoesPNE($inuid, false);

        $perc = (($preenchimento['respondido'] - $preenchimento['qtd_sem_anexo']) * 100) / $preenchimento['total'];
        $perc = ($perc + $percPNE) / 2;

        if ($detalhado) {
            $questoes = $modelUnidade->retornaQuestoesParaPreenchimento($inuid);
            $retorno = array();

            foreach ($questoes as $questao) {
                $retorno[] = array(
                    'descricao' => $questao['pertitulo'],
                    'percent' => $questao['respondido'],
                    'total' => 1,
                    'url' => "par3.php?modulo=principal/planoTrabalho/questoesEstrategicas&acao=A&inuid={$inuid}"
                );
            }

            return $retorno;
        } else {
            return (int)$perc;
        }
    }//end cabecalhoUnidade();

    /**
     * Função verificaPreencimentoQuestoesPNE
     * - verifica o preenchimento da aba dados da unidade e retorna um percentual
     * de preenchimento.
     *
     * @return escreve a lista.
     *
     */
    public function verificaPreencimentoQuestoesPNE($inuid, $detalhado = false)
    {
        $modelUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $preenchimento = $modelUnidade->retornaPreenchimentoQuestoesPNE($inuid);

        if ($preenchimento['total'] > 0) {
            $perc = ($preenchimento['respondido'] * 100) / $preenchimento['total'];
        } else {
            $perc = 0;
        }

        if ($detalhado) {
            $questoes = $modelUnidade->retornaQuestoesParaPreenchimentoPNE($inuid);
            $retorno = array();

            foreach ($questoes as $questao) {
                $retorno[] = array(
                    'descricao' => $questao['pertitulo'],
                    'percent' => $questao['respondido'],
                    'total' => 1,
                    'url' => "par3.php?modulo=principal/planoTrabalho/questoesPNE&acao=A&inuid={$inuid}"
                );
            }

            return $retorno;
        } else {
            return (int)$perc;
        }
    }//end cabecalhoUnidade();

    public function verificaPendencias($arrNot = array())
    {
        $controlPendencia = new Par3_Helper_PendenciasEntidade($_REQUEST['inuid']);

        $pendencia = $controlPendencia->controlePendenciasBox($arrNot);

        if (is_array($pendencia['0']))
            $totalPendencia = count($pendencia['0']);
        else
            $totalPendencia = 0;

        $perc = ($totalPendencia * 100) / (8 - count($arrNot));

        return $perc;
    }

    /**
     * Função desenharTela
     * - desenha a tela da unidade de acordo com o menu a ser preenchido
     *
     * @return escreve a tela.
     *
     */
    public function desenharTela($menu)
    {
        global $disabled;
        $modelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
        $pastaDadosUnidade = 'par3/modulos/principal/planoTrabalho/dadosUnidade/';
        $pastaAcompanhamento = 'par3/modulos/principal/planoTrabalho/acompanhamento/';

        switch ($menu) {
            case'prefeito':
                include APPRAIZ . $pastaDadosUnidade . 'prefeito.php';
                break;
            case'documentoobrapar':
                include APPRAIZ . $pastaAcompanhamento . 'documentoobrapar.php';
                break;
            case'documentoobrapac':
                include APPRAIZ . $pastaAcompanhamento . 'documentoobrapac.php';
                break;
            case'documentopar':
                include APPRAIZ . $pastaAcompanhamento . 'documentopar.php';
                break;
            case'situacaoentrega':
                include APPRAIZ . $pastaAcompanhamento . 'situacaoSiope.inc';
                break;
            case'receitaeducacao':
                include APPRAIZ . $pastaAcompanhamento . 'receitaeducacao.php';
                break;
            case'vinculacaorecursos':
                include APPRAIZ . $pastaAcompanhamento . 'vinculacaorecursos.php';
                break;
            case'dadosveiculos':
                include APPRAIZ . $pastaAcompanhamento . 'dadosveiculos.php';
                break;
            case'responsavel':
                include APPRAIZ . $pastaAcompanhamento . 'responsavel.php';
                break;
            case'contrato':
                include APPRAIZ . $pastaAcompanhamento . '/contrato/contrato.php';
                break;
            case'dadosempresa':
            include APPRAIZ . $pastaAcompanhamento . 'dadosempresa.php';
            break;
            case'despesaseducacao':
                include APPRAIZ . $pastaAcompanhamento . 'despesaseducacao.php';
                break;
            case'secretaria':
                include APPRAIZ . $pastaDadosUnidade . 'secretaria.php';
                break;
            case 'prefeitura':
                include APPRAIZ . $pastaDadosUnidade . 'prefeitura.php';
                break;
            case 'dirigente':
                if($_REQUEST['submenu'] == 'form'){
                    include APPRAIZ . $pastaDadosUnidade . 'dirigente/form.php';break;
                }
                include APPRAIZ . $pastaDadosUnidade . 'dirigente.inc';break;
//                 include APPRAIZ . $pastaDadosUnidade . 'dirigente.php';break;
                break;
            case 'secretario':
                include APPRAIZ . $pastaDadosUnidade . 'secretario.php';
                break;
            case 'equipe':
                include APPRAIZ . $pastaDadosUnidade . 'equipe.php';
                break;
            case 'tecnico':
                include APPRAIZ . $pastaDadosUnidade . 'equipeTecnica.php';
                break;
            case 'nutricionista':
                include APPRAIZ . $pastaDadosUnidade . 'nutricionista.php';
                break;
            case 'comite':
                include APPRAIZ . $pastaDadosUnidade . 'cacs.php';
                break;
            case 'conselho':
                include APPRAIZ . $pastaDadosUnidade . 'conselho.php';
                break;
            case 'cae':
                include APPRAIZ . $pastaDadosUnidade . 'cae.php';
                break;
            default:
                include APPRAIZ . $pastaDadosUnidade . 'prefeitura.php';
                break;
        }

    }//end desenharTela();

//     /**
//      * Função verificaPendencias
//      * - verifica se a unidade possui pendências
//      *
//      * @return $true | false.
//      *
//      */
//     public function verificaPendencias()
//     {
//         global $disabled;
//         $modelUnidade = new Par3_Model_InstrumentoUnidade($_REQUEST['inuid']);
//         $arrDados = $modelUnidade->retornaBooPendencias();

//         return $arrDados['habilitacao']=='t'||$arrDados['cae']=='t'||$arrDados['contas']=='t'||$arrDados['monitoramento_par']=='t'||$arrDados['obras_par']=='t'||$arrDados['cacs']=='t'||$arrDados['pne']=='t'||$arrDados['siope']=='t';
//     }//end desenharTela();

    public function pegarEstUF($inuid)
    {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        return $modelInstrumentoUnidade->estuf;

    }//end pegarItrid();


    public function modalSituacao($dados){

        require_once APPRAIZ.'includes/workflow.php';

        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($dados['inuid']);
        $docid = $modelInstrumentoUnidade->docid;


        $mEstadoDocumento = new Workflow_Model_EstadoDocumento();
        $rsEsd = $mEstadoDocumento->getArrayEstadoDocumentoPAR3();
        $arrayEstadosWorkflow = array_merge(array_keys($rsEsd['Etapa Diagnóstico']), array_keys($rsEsd['Etapa Planejamento']));

        #estado origem - documento atual
        $estadoAtual = wf_pegarEstadoAtual($docid, 0);

        #estado destino - estado anterior do fluxo.
        $key = array_search($estadoAtual['esdid'], $arrayEstadosWorkflow);
        $esdidAnterior =  $arrayEstadosWorkflow[$key-1];
        $estadoAnterior =  $esdidAnterior ? $modelInstrumentoUnidade->pegarNomeEstado($esdidAnterior) : ' - ';




        #dados da unidade
        $nome = self::pegarNomeEntidade($dados['inuid']);
        $esfera = ucfirst(self::pegarDescricaoEsferaDif($dados['inuid']));
        $dadosHistorico = wf_pegarHistorico($docid);
        $aedid = wf_pegarAcao($estadoAtual['esdid'], $esdidAnterior);

        //Só pra adicionar a coluna 'sequencia' no array
        foreach ($dadosHistorico as $key => $value){
            $dadosHistorico[$key] = array('id'=>$key) + $dadosHistorico[$key];
        }

        ob_start();

        echo "
        <div>Situação Atual: <br>
            <b> " . $estadoAtual['esddsc'] . " </b>
        </div>
        <br>
        <div>Enviar para: <br>
            <b> " . $estadoAnterior . "</b>
        </div>
        <br>
        <input type='hidden' name='requisicao' value='tramitarSituacao'>
        <input type='hidden' name='aedid' value='" . $aedid['aedid'] . "'>
        <input type='hidden' name='docid' value='" . $docid . "'>
        <button style=\"width:100%;font-size:10px;padding:5px!important;\"
                onclick=\"toggleHistorico();\"
                type=\"button\" class=\"btn btn-info\">
                <span class=\"glyphicon glyphicon-time\" aria-hidden=\"true\"></span> Histórico
            </button>
         <div id='listaHistorico' style='display: none;'>";
        $listagem = new Simec_Listagem();
        $listagem->setCabecalho(['Seq.', 'Onde Estava', 'O que aconteceu', 'Quem fez', 'Quando fez']);
        $listagem->setDados($dadosHistorico);
        $listagem->esconderColunas('cmddsc');
        $listagem->setId('historicowf');
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
        echo "</div>
        
        <script>
            $(function(){
            //Escondendo os botões de exportação da lista
                $('#listaHistorico nav').css('display', 'none');
            });
        
            function toggleHistorico(){
                $('#listaHistorico').toggle(200, 'linear');
            }
        </script>
        ";

        $html = ob_get_clean();

        $arrayData = simec_json_encode(['nome' => $nome, 'esfera' => $esfera, 'esdid' => $esdidAnterior, 'estadoAnterior' => $estadoAnterior]);
        $response = simec_json_encode(['html' => $html, 'dados' => $arrayData]);

        echo $response;
    }


    public function tramitarSituacao($dados){
        require_once APPRAIZ.'includes/workflow.php';

        if(wf_alterarEstado($dados['docid'], $dados['aedid'], '', ['cache_estado_atual'=>0])){
            simec_redirecionar('par3.php?modulo=' . $dados['modulo'] . '&acao=A', 'success');
        }else{
            simec_redirecionar('par3.php?modulo=' . $dados['modulo'] . '&acao=A', 'error', "Não foi possível alterar o a situação");
        }
    }



}//end class

?>
