<?php
namespace Simec\Par3\Controle;

use Simec\Par3\Ajudante\Menu\MenuEntidade;
use Simec\Par3\Modelo\VmRelatorioQuantitativoPendencias as modeloVmRelatorioQuantitativoPendencias;
use Simec\Par3\Modelo\RegraBloqueio as modeloRegraBloqueio;
use Simec\Par3\Modelo\Pendencias as modeloPendencias;
use Simec\Par3\Modelo\Instrumento\Unidade as modeloInstrumentoUnidade;
use Simec\Par3\Modelo\Cae\Cae as modeloCae;
use Simec\Par3\Modelo\Siope as modeloSiope;
use Simec\Par3\Modelo\Restricao as modeloRestricao;

/**
 * Classe controladora responsável pela exibição das pendências das entidades
 * @author felipe.tcc@gmail.com
 */
class Pendencia extends \Simec\AbstractControle
{

//     use \Simec\Par3\ControleTrait;

    const PENDENCIA_PAR         = "Pendências do PAR";
    const PENDENCIA_ALERTA      = "Alertas!";
    const PENDENCIA_PARABENS    = "Parabéns!";
    
    public $arTipoBloqueio;
    
    public function finish()
    {
        
    }

    public function init()
    {
        $this->arTipoBloqueio = [
            1 => 'cae',
            2 => 'cacs',
            3 => 'habilitacao',
            4 => 'monitoramento_par',
            5 => 'obras_par',
            6 => 'pne',
            7 => 'contas',
            8 => 'siope',
        ];
        
    }

    public function index()
    {
        $this->js('/zimec/public/temas/simec/js/plugins/viewjs/view.js');
        
        $dadoPendencia = (new modeloVmRelatorioQuantitativoPendencias())->listarPendenciaPorInuid($this->get('inuid'));
        
        $this->tratarDataCargaPendencia($dadoPendencia['dtcarga']);
        
        $this->toJs('inuid', $this->get('inuid'));
        
        $this->toView('menuNavegacaoEntidade', (new MenuEntidade())->menu());
        $this->toView('nomeUnidade', $this->pegarNomeUnidade($this->get('inuid')));
        $this->toView('listaQuadroPendencia', $this->montarAgrupamentoTipoPendencia($dadoPendencia,(new modeloRegraBloqueio())->listarBloqueio()));
        
        $this->showHtml();
    }
    
    public function exibirSubDetalhePendencia()
    {
        $pendencias = new modeloPendencias();
        $instrumentoUnidade   = new modeloInstrumentoUnidade();
        $dados      = array_merge($this->get(), $this->post());
        
        $dados['inuid_par'] = $instrumentoUnidade->recuperarInuidPar($this->get('inuid'));
        
        switch($dados['funcao']){
            case 'item_sem_contrato':
                $dados = $pendencias->buscaDetalheItemSemContrato($dados);
                break;
            case 'item_contrato_sem_qtd_recebido':
                $dados = $pendencias->buscaDetalheItemContratoSqmQtdRecebido($dados);
                break;
            case 'item_sem_detalhar':
                $dados = $pendencias->buscaDetalheItemSemDetalhar($dados);
                break;
            case 'item_detalhado_menor_recebido':
                $dados = $pendencias->buscaDetalheItemDetalhadoMenosRecebido($dados);
                break;
            case 'item_sem_nota':
                $dados = $pendencias->buscaDetalheItemSemNota($dados);
                break;
            case 'contrato_diligencia':
                $dados = $pendencias->buscaDetalheContratoDiligencia($dados);
                break;
            case 'nota_diligencia':
                $dados = $pendencias->buscaDetalheNotaDiligencia($dados);
                break;
            case 'termos_n_assinados':
                $dados = $pendencias->buscaDetalheTermosNAssinados($dados);
                break;
        }
        
        $this->showJson(($dados ? $dados : []));
    }
    
    public function exibirDetalhamentoPendencia()
    {
//         $_POST['pendencia'] = 'monitoramento_par';
//         $_POST['inuid'] = 5488;
        
        $pendencia = ucfirst($this->post('pendencia'));
        //         $pendencia = 'monitoramento_par';
        $this->{"tipoPendencia{$pendencia}"}();
    }
    
    protected function tipoPendenciaHabilitacao()
    {
        $inuid          = $this->get('inuid');
        $dadoPendencia  = (new modeloVmRelatorioQuantitativoPendencias())->listarPendenciaPorInuid($inuid);
        
        $existePendencia = ($dadoPendencia['habilitacao'] == 't' ? true : false);
        
        $arConfigCaixaPendencia = $this->parametrosDetalheCaixaPendencia(
                                    $this->post('pendencia'),
                                    $existePendencia,
                                    $this->classificarPendencia()
                                  );
        $arConfigCaixaPendencia['nomeEntidade'] = $this->pegarNomeUnidade($inuid);
        
        $this->showJson($arConfigCaixaPendencia);
    }
    
    protected function tipoPendenciaContas()
    {
        $inuid          = $this->get('inuid');
        $dadoPendencia  = (new modeloVmRelatorioQuantitativoPendencias())->listarPendenciaPorInuid($inuid);
        
        $existePendencia = ($dadoPendencia['contas'] == 't' ? true : false);
        
        $arConfigCaixaPendencia = $this->parametrosDetalheCaixaPendencia(
            $this->post('pendencia'),
            $existePendencia,
            $this->classificarPendencia()
            );
        $arConfigCaixaPendencia['nomeEntidade'] = $this->pegarNomeUnidade($inuid);
        
        if ($existePendencia == 't') {
            $instrumentoUnidade = new modeloInstrumentoUnidade();
            $arConfigCaixaPendencia['pendencia'] = $instrumentoUnidade->pegarPendenciasPrestacaoContas($inuid);
        }
        $arConfigCaixaPendencia['pendencia'] = ($arConfigCaixaPendencia['pendencia'] 
                                                    ? $arConfigCaixaPendencia['pendencia']
                                                    : array());
        
        $this->showJson($arConfigCaixaPendencia);
    }    
    
    protected function tipoPendenciaSiope()
    {
        $inuid          = $this->get('inuid');
        $dadoPendencia  = (new modeloVmRelatorioQuantitativoPendencias())->listarPendenciaPorInuid($inuid);
        
        $existePendencia = ($dadoPendencia['siope'] == 't' ? true : false);
        
        if ($existePendencia) {
            $oSiope = new modeloSiope();
            $resultado = $oSiope->transmissaoSiope($_REQUEST);
//             $transmissao = $resultado['0']['cod_situacao'];
            $msgPendenciaSiope = $resultado['0']['situacao_desc'];
            
        }
        
        $arConfigCaixaPendencia = $this->parametrosDetalheCaixaPendencia(
                                    $this->post('pendencia'),
                                    $existePendencia,
                                    $this->classificarPendencia(),
                                    $msgPendenciaSiope
                                  );
        $arConfigCaixaPendencia['nomeEntidade'] = $this->pegarNomeUnidade($inuid);
        
        $this->showJson($arConfigCaixaPendencia);
    }    
    
    protected function tipoPendenciaPne()
    {
        $inuid          = $this->get('inuid');
        $dadoPendencia  = (new modeloVmRelatorioQuantitativoPendencias())->listarPendenciaPorInuid($inuid);
        
        $existePendencia = ($dadoPendencia['pne'] == 't' ? true : false);
        
        $arConfigCaixaPendencia = $this->parametrosDetalheCaixaPendencia(
                                            $this->post('pendencia'),
                                            $existePendencia,
                                            $this->classificarPendencia()
                                         );
        
        $instrumentoUnidade   = new modeloInstrumentoUnidade($inuid);
        
        $arConfigCaixaPendencia['description'] = ($instrumentoUnidade->dados->itrid == 2 
                                                    ? "Plano Municipal de Educação"
                                                    : "Plano Estadual de Educação");
        $arConfigCaixaPendencia['nomeEntidade'] = $this->pegarNomeUnidade($inuid);
        
        $this->showJson($arConfigCaixaPendencia);
    }
    
    protected function tipoPendenciaObras_par()
    {
        $inuid          = $this->get('inuid');
        $dadoPendencia  = (new modeloVmRelatorioQuantitativoPendencias())->listarPendenciaPorInuid($inuid);
        $tipoPendencia  = $this->post('pendencia');
        /*
         * APAGAR   
         */
//         $tipoPendencia = 'obras_par'; 
        $arConfigCaixaPendencia = $this->parametrosDetalheCaixaPendencia(
                                    $tipoPendencia, 
                                    ($dadoPendencia['obras_par'] == 't' ? true : false), 
                                    $this->classificarPendencia()
                                  );
        $arConfigCaixaPendencia['nomeEntidade'] = $this->pegarNomeUnidade($inuid);
        $arConfigCaixaPendencia['pendencia']    = array();
        $arConfigCaixaPendencia['restricao']    = array();
        
        if ($dadoPendencia['obras_par'] == 't') {
            $arConfigCaixaPendencia['pendencia'] = $this->recuperarPendenciasEntidade($inuid);
            
            $modelRestricao = new modeloRestricao();
            $arRestricao = $modelRestricao->getObrasRestricoesInstrumentoUnidadeById($inuid);
            $arRestricao = ($arRestricao ? $arRestricao : array());
            
            for ($i = 0; $i < count($arRestricao); $i++) {
                $arDetalheRestricao = $modelRestricao->getRestricoesDaObra($arRestricao[$i]['obrid']);
                $arDetalheRestricao = ($arDetalheRestricao ? $arDetalheRestricao : array());
                foreach ($arDetalheRestricao as $detalheRestricao) {
                    $arRestricao[$i]['detalhe-item-restricao'][] = $detalheRestricao['rstdsc'];
                }
            }
            
            $arConfigCaixaPendencia['restricao'] = $arRestricao;
        }
        
        $this->showJson($arConfigCaixaPendencia);
    }
    
    protected function recuperarPendenciasEntidade($inuid)
    {
        $pendencias = new modeloPendencias();
        $instrumentoUnidade   = new modeloInstrumentoUnidade($inuid);
        $estuf  = $instrumentoUnidade->dados->estuf;
        $muncod = $instrumentoUnidade->dados->muncod;
        $itrid  = $instrumentoUnidade->dados->itrid;
        
        $arPendencia = $pendencias->recuperarPendenciasEntidade($inuid, $estuf, $muncod, $itrid);
        $arPendencia = ($arPendencia ? $arPendencia : array());
        
        return $arPendencia;
    }
    
    protected function tipoPendenciaMonitoramento_par()
    {
        $inuid          = $this->get('inuid');
        $dadoPendencia  = (new modeloVmRelatorioQuantitativoPendencias())->listarPendenciaPorInuid($inuid);
        
        $existePendencia = ($dadoPendencia['monitoramento_par'] == 't' ? true : false);
        
        $arConfigCaixaPendencia = $this->parametrosDetalheCaixaPendencia(
                                    $this->post('pendencia'),
                                    $existePendencia,
                                    $this->classificarPendencia()
                                  );
        $arConfigCaixaPendencia['nomeEntidade'] = $this->pegarNomeUnidade($inuid);

        $arConfigCaixaPendencia['regraPagamento']               = array();
        $arConfigCaixaPendencia['saldoConta']                   = array();
        $arConfigCaixaPendencia['regraMonitoramento2010']       = array();
        $arConfigCaixaPendencia['regraMonitoramento2010Termo']  = array();
        
        if ($existePendencia) {
            $pendencias         = new modeloPendencias();
            $instrumentoUnidade = new modeloInstrumentoUnidade($inuid);
            
            $estuf  = $instrumentoUnidade->dados->estuf;
            $muncod = $instrumentoUnidade->dados->muncod;
            $itrid  = $instrumentoUnidade->dados->itrid;
            
            $arConfigCaixaPendencia['regraPagamento'] = $pendencias->recuperaProcessosRegraPagamento($inuid, $estuf, $muncod , $itrid );
            $arConfigCaixaPendencia['regraPagamento'] = ($arConfigCaixaPendencia['regraPagamento'] 
                                                            ? $arConfigCaixaPendencia['regraPagamento']
                                                            : array());
            $arConfigCaixaPendencia['saldoConta'] = $pendencias->recuperaProcessosRegraSaldoConta($inuid, $estuf, $muncod , $itrid );
            $arConfigCaixaPendencia['saldoConta'] = ($arConfigCaixaPendencia['saldoConta'] 
                                                        ? $arConfigCaixaPendencia['saldoConta'] 
                                                        : array());
            $arConfigCaixaPendencia['regraMonitoramento2010'] = $pendencias->recuperaProcessosRegraMonitoramento2010($inuid);
            $arConfigCaixaPendencia['regraMonitoramento2010'] = ($arConfigCaixaPendencia['regraMonitoramento2010']
                                                                    ? $arConfigCaixaPendencia['regraMonitoramento2010']
                                                                    : array());
            
            $arConfigCaixaPendencia['regraMonitoramento2010Termo'] = $pendencias->recuperaProcessosRegraMonitoramento2010Termos($inuid);
            $arConfigCaixaPendencia['regraMonitoramento2010Termo'] = ($arConfigCaixaPendencia['regraMonitoramento2010Termo']
                                                                    ? $arConfigCaixaPendencia['regraMonitoramento2010Termo']
                                                                    : array());
        }
        
        $this->showJson($arConfigCaixaPendencia);
    }
    
    protected function tipoPendenciaCae()
    {
        $inuid = $this->get('inuid');
        
        $modelCAE     = new modeloCae();
        $conselho     = $modelCAE->carregarDadosCAE($inuid);
        $presidente   = $modelCAE->carregarDadosPresidenteCAE($inuid);
        $conselheiros = $modelCAE->carregarConselheirosCAE($inuid);
        
        $existePendencia = ((!empty($conselho->arqid) && !empty($presidente->capid) && is_array($conselheiros) && count($conselheiros) >= 6)
                        ?
                            0
                        :
                            1);
                
        $arConfigCaixaPendencia = $this->parametrosDetalheCaixaPendencia(
                                    $this->post('pendencia'), 
                                    $existePendencia, 
                                    $this->classificarPendencia()
                                  );
        $arConfigCaixaPendencia['nomeEntidade'] = $this->pegarNomeUnidade($inuid);
        $this->showJson($arConfigCaixaPendencia);
    }
    
    protected function tipoPendenciaCacs()
    {
        $inuid          = $this->get('inuid');
        $dadoPendencia  = (new modeloVmRelatorioQuantitativoPendencias())->listarPendenciaPorInuid($inuid);
        
        $existePendencia = ($dadoPendencia['cacs'] == 't' ? true : false);
        
        $arConfigCaixaPendencia = $this->parametrosDetalheCaixaPendencia(
                                    $this->post('pendencia'),
                                    $existePendencia,
                                    $this->classificarPendencia()
                                  );
        $arConfigCaixaPendencia['nomeEntidade'] = $this->pegarNomeUnidade($inuid);
        
        $this->showJson($arConfigCaixaPendencia);
    }
    
    /**
     * Fas o tratamento da data de carga das pendências da unidade e envia os dados para a view
     * @param DateTime $dtCarga
     * @return void
     */
    protected function tratarDataCargaPendencia($dtCarga)
    {
        if ($dtCarga) {
            preg_match('/(\d{4})\-(\d{2})\-(\d{2})(.*)\.\d*/', $dtCarga, $m);
            $dataUltimaAtualizacao = "{$m[3]}/{$m[2]}/{$m[1]}{$m[4]}";
            preg_match('/(\d{4})\-(\d{2})\-(\d{2}) (\d+):(\d+):(\d+)\.\d*/', $dtCarga, $p);
            $maisDeDozeHoras = (mktime(date('H'), date('i'), date('s'), date('n'), date('j'), date('Y')) -
                mktime((integer)$p[4],(integer)$p[5],(integer)$p[6],(integer)$p[2],(integer)$p[3],(integer)$p[1])) > 43200;
        } else {
            $dataUltimaAtualizacao = '';
            $maisDeDozeHoras = false;
        }
        $this->toView('dataUltimaAtualizacao', $dataUltimaAtualizacao);
        $this->toView('maisDeDozeHoras', $maisDeDozeHoras);
    }
        
    /**
     * Busca o nome da unidade na tabela: par3.instrumentounidade
     * @param integer $inuid
     * @return string
     */
    protected function pegarNomeUnidade($inuid)
    {
        $instrumentoUnidade   = new modeloInstrumentoUnidade($inuid);
        
        $entidade = $instrumentoUnidade->dados->inudescricao;
        $estuf = $instrumentoUnidade->dados->estuf;
        $itrid = $instrumentoUnidade->dados->itrid;
        
        return (($itrid == 1) ? $entidade : $entidade . ' - ' . $estuf);
    }
    
    /**
     * Monta o agrupamento dos tipos pendências para a unidade, classificando como: Pendências do PAR, Alertas! 
     * ou Parabéns!
     * @param array $dadoPendencia
     * @param array $listaRegraBloqueio
     * @return array
     */
    protected function montarAgrupamentoTipoPendencia(Array $dadoPendencia, Array $listaRegraBloqueio)
    {
//         $arGrupoPendencia = ["Pendências do PAR"=>[], "Alertas!"=>[], "Parabéns!"=> []];
        $arGrupoPendencia = [self::PENDENCIA_PAR=>[], self::PENDENCIA_ALERTA=>[], self::PENDENCIA_PARABENS=> []];
        
        $deParaId = $this->arTipoBloqueio;
        
        foreach ($listaRegraBloqueio as $k => $regraBloqueio) {
            if ($dadoPendencia[$deParaId[$regraBloqueio['id']]] == 'f') {
                $arGrupoPendencia[self::PENDENCIA_PARABENS][] = $this->parametrosCaixaPendencia($deParaId[$regraBloqueio['id']], false);
            } else {
                $classificacaoPendencia = $this->classificarPendencia($regraBloqueio);
                $arGrupoPendencia[$classificacaoPendencia][] = $this->parametrosCaixaPendencia(
                                                                    $deParaId[$regraBloqueio['id']], 
                                                                    true, 
                                                                    $classificacaoPendencia
                                                               );
            }
        }
        
        return $arGrupoPendencia;
    }
    
    protected function classificarPendencia($regraBloqueio=null, $tipoPendencia=null)
    {
        if (!$regraBloqueio) {
            $tipoPendencia  = ($tipoPendencia ? $tipoPendencia : $this->post('pendencia'));
            $rebid = array_search($tipoPendencia, $this->arTipoBloqueio);
            $regraBloqueio = (new modeloRegraBloqueio())->pegarBloqueioPorId($rebid);
        }
        
        return ((
                    $regraBloqueio['planejamento'] == 't' ||
                    $regraBloqueio['empenho'] == 't' ||
                    $regraBloqueio['termo'] == 't' ||
                    $regraBloqueio['pagamento'] == 't'
                )
            ? self::PENDENCIA_PAR
            : self::PENDENCIA_ALERTA);
    }
    
    /**
     * Monta a configuração das caixas de exibição dos tipos de pendências: Pendências do PAR, Alertas! ou Parabéns!
     * @param string $pendencia
     * @param boolean $existePendencia
     * @param string $classificacaoPendencia
     * @return array
     */
    protected function parametrosCaixaPendencia($pendencia, $existePendencia = false, $classificacaoPendencia = '')
    {
        $arAux = array();
        
        switch ($pendencia) {
            case 'obras_par':
                $arAux['description'] = "Pendências de Obras do PAR";
                $arAux['widget'] = "#ed5565";
                $arAux['icon'] = "fa-building";
                break;
            case 'habilitacao':
                $arAux['description'] = "Habilitação";
                $arAux['widget'] = "#BDB76B";
                $arAux['icon'] = "fa-certificate";
                break;
            case 'cacs':
                $arAux['description'] = "<span style=\"font-size: 9pt;\">Cons. de Acomp. de Controle Social</span>";
                $arAux['widget'] = "#f8ac59";
                $arAux['icon'] = "fa-group";
                break;
            case 'cae':
                $arAux['description'] = "Conselho de Alimentação Escolar";
                $arAux['widget'] = "#EEC591";
                $arAux['icon'] = "fa-cutlery";
                break;
            case 'contas':
                $arAux['description'] = "Prestação de Contas";
                $arAux['widget'] = "#23c6c8";
                $arAux['icon'] = "fa-dollar";
                break;
            case 'monitoramento_par':
                $arAux['description'] = "Monitoramento PAR 2011-2014 e Termos de Compromisso";
                $arAux['widget'] = "#8B8989";
                $arAux['icon'] = "fa-binoculars";
                break;
            case 'siope':
                $arAux['description'] = "SIOPE";
                $arAux['widget'] = "#4A708B";
                $arAux['icon'] = "fa-desktop";
                break;
            case 'pne':
                $arAux['description'] = "Plano Municipal de Educação";
                $arAux['widget'] = "#BC8F8F";
                $arAux['icon'] = "fa-graduation-cap";
                break;
        }
        
        if ($existePendencia && $classificacaoPendencia == 'Pendências do PAR') {
            $arAux['thumb'] = "fa-times-circle";
            $arAux['boolean'] = 1;
        } elseif ($existePendencia) {
            $arAux['thumb'] = "fa-warning";
            $arAux['boolean'] = 1;
        } else {
            $arAux['thumb'] = "fa-thumbs-up";
            $arAux['boolean'] = 0;
        }
        $arAux['type'] = $pendencia;
        
        return $arAux;
    }

    protected function parametrosDetalheCaixaPendencia(
        $pendencia, 
        $existePendencia = false, 
        $classificacaoPendencia = '', 
        $msgPendenciaSiope = null
    ) {
        $arAux = array();
        
        switch($pendencia){
            
            case 'obras_par':
                $arAux['description'] = "Pendências de Obras do PAR";
                $arAux['widget'] = "#ed5565";
                $arAux['icon'] = "fa-building";
                $arAux['msgSucess'] = "Não existem pendências de Obras do PAR.";
                $arAux['msgFail'] = "Existem pendências de Obras do PAR.";
                break;
            case 'habilitacao':
                $arAux['description'] = "Habilitação";
                $arAux['widget'] = "#BDB76B";
                $arAux['icon'] = "fa-certificate";
                $arAux['msgSucess'] = "Entidade encontra-se habilitada";
                $arAux['msgFail'] = "Entidade não encontra-se habilitada";
                break;
            case 'cacs':
                $arAux['description'] = "Conselho de Acompanhamento de Controle Social - CACS";
                $arAux['widget'] = "#f8ac59";
                $arAux['icon'] = "fa-group";
                $arAux['msgSucess'] = "A situação no CACS está REGULAR.";
                $arAux['msgFail'] = "A situação no CACS está IRREGULAR.";
                break;
            case 'cae':
                $arAux['description'] = "Conselho de Alimentação Escolar";
                $arAux['widget'] = "#EEC591";
                $arAux['icon'] = "fa-cutlery";
                $arAux['msgSucess'] = "A etapa de preenchimento do Conselho de Alimentação Escolar encontra-se preenchida.";
                $arAux['msgFail'] = "A etapa de preenchimento do Conselho de Alimentação Escolar não encontra-se preenchida.";
                break;
            case 'contas':
                $arAux['description'] = "Prestação de Contas";
                $arAux['widget'] = "#23c6c8";
                $arAux['icon'] = "fa-dollar";
                $arAux['msgSucess'] = "Não existem pendências na Prestação de contas.";
                $arAux['msgFail'] = "Existem pendências na Prestação de Contas.";
                break;
            case 'monitoramento_par':
                $arAux['description'] = "Monitoramento PAR";
                $arAux['widget'] = "#8B8989";
                // 				$arAux['widget'] = "#1ab394";
                $arAux['icon'] = "fa-binoculars";
                $arAux['msgSucess'] = "Não existem pendencias no Monitoramento PAR.";
                $arAux['msgFail'] = "Existem pendências no Monitoramento PAR.";
                break;
            case 'siope':
                $arAux['description'] = "SIOPE";
                $arAux['widget'] = "#4A708B";
                // 				$arAux['widget'] = "#1c84c6";
                $arAux['icon'] = "fa-desktop";
                $arAux['msgSucess'] = "Não existem pendências no SIOPE.";
                $arAux['msgFail'] = "Existem pendências no SIOPE. <br/>" . $msgPendenciaSiope;
                break;
            case 'pne':
                $arAux['description'] = "Plano Municipal de Educação";
                $arAux['widget'] = "#BC8F8F";
                $arAux['icon'] = "fa-graduation-cap";
                $arAux['link'] = "<span id=\"pneArquivo\" style=\"color: #53868B; font-weight: bold; \"> Clique Aqui </span> para download ";
                $arAux['msgSucess'] = "O arquivo com a lei Municipal/Estadual encontra-se disponível. ";
                $arAux['msgFail'] = "O arquivo com a lei Municipal/Estadual não encontra-se disponível.";
                break;
                
        }
        
        if ($existePendencia && $classificacaoPendencia == 'Pendências do PAR') {
            $arAux['thumb'] = "fa-times-circle";
            $arAux['boolean'] = 1;
            $arAux['msg'] = $arAux['msgFail'];
            $arAux['link'] = '';
        } elseif ($existePendencia) {
            $arAux['thumb'] = "fa-warning";
            $arAux['boolean'] = 1;
            $arAux['msg'] = $arAux['msgFail'];
            $arAux['link'] = '';
        } else {
            $arAux['thumb'] = "fa-thumbs-up";
            $arAux['boolean'] = 0;
            $arAux['msg'] = $arAux['msgSucess'];
        }
        $arAux['type'] = $pendencia;
        
        return $arAux;
    }
}
