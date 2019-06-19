<?php
/**
 * @version $Id: Programacaocompleta.php 147086 2018-12-19 19:08:24Z victormachado $
 * Date: 01/11/2017
 * Time: 16:57
 */

include_once APPRAIZ . "spo/ws/siop/Quantitativo.php";

class Spo_Job_Qt_Programacaocompleta extends Simec_Job_Abstract
{
    const TAMANHO_PAGINA = 2000;

    /**
     * @var array
     */
    private $mapa;

    /**
     * @var Spo_Ws_Sof_Quantitativo
     */
    private $ws;

    /**
     * @var array
     */
    private $params;

    /**
     * @return string
     */
    public function getName()
    {
        return 'Baixando Dados Quantitativos';
    }

    protected function init()
    {
        $this->params = array_merge(
            [
                'pagina'        => '0'
            ],
            $this->loadParams()
        );

        $this->mapa = [
            'proposta'                        => $this->montarNomeTabela('propostadto'),
            'financeiro'                      => $this->montarNomeTabela('financeirodto'),
            'metaPlanoOrcamentario'           => $this->montarNomeTabela('metaplanoorcamentariodto'),
            'acoesacompanhamentoorcamentario' => $this->montarNomeTabela('acoesacompanhamentoorcamentariodto'),
            'receita'                         => $this->montarNomeTabela('receitadto')
        ];

//        $this->ws = new Spo_Ws_Sof_Quantitativo($this->params['log'], Simec_BasicWS::PRODUCTION);
    }

    protected function execute()
    {
        try {
            $params = $this->params;

            $count = 0;

            $bloco = self::TAMANHO_PAGINA;
            echo "Processamento de carga por bloco de {$bloco} registros.<br>";

            do {
                // Montar objeto de paginação
//                $paginacao = $this->montarPaginacao($params['pagina']);

                // fazer chamadoa ao Siop
                $retornoSiop = $this->conexaoSiop($params);

                // Limpar tabelas de carga
                $this->limparTabelas($params);

                // mensagem de acompanhamento
                $pagina = $params['pagina'] + 1;
                echo "Processando o bloco {$pagina}.<br>";

                $this->salvarOutput();

                // persistir os dados retornados do Siop
                $count += $this->persistirDados($retornoSiop);

                // verifica se possui proxima pagina
                $params = $this->isUltimaPagina($params, $retornoSiop, $paginacao);
            } while (!$params['terminate']);

            echo "<br>Inserido no total {$count} registros.";
        } catch (Exception $e) {
            throw $e;
        }
    }

    protected function shutdown()
    {
    }

    /**
     * @param $pagina
     *
     * @return PaginacaoDTO
     */
    private function montarPaginacao($pagina)
    {
        $paginacao = new PaginacaoDTO();
        $paginacao->registrosPorPagina = self::TAMANHO_PAGINA;
        $paginacao->pagina = $pagina;

        return $paginacao;
    }

    /**
     * @param $params
     * @param $paginacao
     *
     * @return ObterProgramacaoCompletaQuantitativo
     */
    private function montarObterProgramacao($params, $paginacao)
    {
        $filtro = new ObterProgramacaoCompletaQuantitativo();
        $filtro->exercicio = $params['exercicio'];
        $filtro->codigoMomento = $params['codigomomento'];
        $filtro->paginacao = $paginacao;

        return $filtro;
    }

    /**
     * @param $params
     * @param $paginacao
     *
     * @return obterProgramacaoCompletaQuantitativoResponse
     * @throws Exception
     */
    private function conexaoSiop($params, $paginacao = null)
    {
        try {
//            $retornoSiop = $this->ws->obterProgramacaoCompletaQuantitativo($this->montarObterProgramacao($params, $paginacao));
            $retornoSiop = (new Spo_Ws_Siop_Quantitativo())
                ->setSchema('acomporc')
                ->obterProgramacaoCompletaQuantitativo(
                    $params['exercicio'],
                    $params['codigomomento']
                );

            if ($retornoSiop instanceof SoapFault) {
                throw new Exception($retornoSiop->getMessage());
            }

            $retornoSiop = $retornoSiop->return;

            if ($retornoSiop->mensagensErro) {
                $mensagens = [];

                foreach ((array) $retornoSiop->mensagensErro as $message) {
                    $mensagens[] = $message;
                }

                throw new Exception(implode('<br>', $mensagens));
            }

            return $retornoSiop;
        } catch(Exception $e) {
            throw $this->gerarXml('obterProgramacaoCompletaQuantitativo', $retornoSiop, $this->params['log'], $e);
        }
    }

    /**
     * @param $retornoSiop
     *
     * @return int
     *
     * @throws Exception
     */
    private function persistirDados($retornoSiop)
    {
        if (!count($retornoSiop->proposta)) {
            return 0;
        }

        $insert = '';
        $count = 0;

        try {
            foreach ($retornoSiop->proposta as $proposta) {
                $insert .= $this->insertPropostaDto($proposta);
                $insert .= $this->insertPropostaDtoFinanceiro($proposta->financeiros);
                $insert .= $this->insertPropostaDtoMetaPlanoorcamentario($proposta->metaPlanoOrcamentario);
                $insert .= $this->insertPropostaDtoReceita($proposta->receitas);

                $count++;
            }

            $this->executar($insert);

            $this->commit();
        } catch (Exception $e) {
            throw $e;
        }

        return $count;
    }

    /**
     * @param $proposta
     *
     * @return string
     */
    private function insertPropostaDto($proposta)
    {
        $insert = <<<DML
INSERT INTO {$this->mapa['proposta']}(codigoacao,codigoesfera,codigofuncao,codigolocalizador,codigomomento,codigoorgao,codigoprograma,
                                 codigosubfuncao,codigotipodetalhamento,codigotipoinclusaoacao,codigotipoinclusaolocalizador,
                                 exercicio,expansaofisicaconcedida,expansaofisicasolicitada,identificadorunicoacao,justificativa,
                                 justificativaexpansaoconcedida,justificativaexpansaosolicitada,quantidadefisico,snatual,valorfisico)
  VALUES ('{$proposta->codigoAcao}',
          '{$proposta->codigoEsfera}',
          '{$proposta->codigoFuncao}',
          '{$proposta->codigoLocalizador}',
          '{$proposta->codigoMomento}',
          '{$proposta->codigoOrgao}',
          '{$proposta->codigoPrograma}',
          '{$proposta->codigoSubFuncao}',
          '{$proposta->codigoTipoDetalhamento}',
          '{$proposta->codigoTipoInclusaoAcao}',
          '{$proposta->codigoTipoInclusaoLocalizador}',
          '{$proposta->exercicio}',
          '{$proposta->expansaoFisicaConcedida}',
          '{$proposta->expansaoFisicaSolicitada}',
          '{$proposta->identificadorUnicoAcao}',
          '{$proposta->justificativa}',
          '{$proposta->justificativaExpansaoConcedida}',
          '{$proposta->justificativaExpansaoSolicitada}',
          '{$proposta->quantidadeFisico}',
          '{$proposta->snAtual}',
          '{$proposta->valorFisico}');
DML;

        return $insert;
    }

    /**
     * @param $financeiro
     *
     * @return string
     */
    private function insertPropostaDtoFinanceiro($financeiro)
    {
        if (empty($financeiro)) {
            return '';
        }

        $insert = '';
        if (is_array($financeiro)) {
            foreach ($financeiro as $fin) {
                $insert .= $this->insertPropostaDtoFinanceiro($fin);
            }

            return $insert;
        }

        $insert = <<<DML
INSERT INTO {$this->mapa['financeiro']}(codigoplanoorcamentario,expansaoconcedida,expansaosolicitada,fonte,idoc,
                                   iduso,identificadorplanoorcamentario,naturezadespesa,resultadoprimarioatual,
                                   resultadoprimariolei,valor)
  VALUES ('{$financeiro->codigoPlanoOrcamentario}',
          '{$financeiro->expansaoConcedida}',
          '{$financeiro->expansaoSolicitada}',
          '{$financeiro->fonte}',
          '{$financeiro->idOC}',
          '{$financeiro->idUso}',
          '{$financeiro->identificadorPlanoOrcamentario}',
          '{$financeiro->naturezaDespesa}',
          '{$financeiro->resultadoPrimarioAtual}',
          '{$financeiro->resultadoPrimarioLei}',
          '{$financeiro->valor}');
DML;

        return $insert;
    }

    /**
     * @param $metaPlanoorcamentario
     *
     * @return string
     */
    private function insertPropostaDtoMetaPlanoorcamentario($metaPlanoorcamentario)
    {
        if (empty($metaPlanoorcamentario)) {
            return '';
        }

        $insert = '';
        if (is_array($metaPlanoorcamentario)) {
            foreach ($metaPlanoorcamentario as $meta) {
                $insert .= $this->insertPropostaDtoMetaPlanoorcamentario($meta);
            }

            return $insert;
        }

        $insert = <<<DML
INSERT INTO {$this->mapa['metaPlanoOrcamentario']}(expansaofisicaconcedida, expansaofisicasolicitada, identificadorunicoplanoorcamentario, quantidadefisico)
  VALUES ('{$metaPlanoorcamentario->expansaoFisicaConcedida}',
          '{$metaPlanoorcamentario->expansaoFisicaSolicitada}',
          '{$metaPlanoorcamentario->identificadorUnicoPlanoOrcamentario}',
          '{$metaPlanoorcamentario->quantidadeFisico}');
DML;

        return $insert;
    }

    /**
     * @param $receita
     *
     * @return string
     */
    private function insertPropostaDtoReceita($receita)
    {
        if (empty($receita)) {
            return '';
        }

        $insert = '';
        if (is_array($receita)) {
            foreach ($receita as $rec) {
                $insert .= $this->insertPropostaDtoReceita($rec);
            }

            return $insert;
        }

        return "INSERT INTO {$this->mapa['receita']}(naturezareceita, valor) VALUES ('{$receita->naturezaReceita}', '{$receita->valor}');";
    }

    private function montarNomeTabela($sufixo)
    {
        return $this->nomeTabela($this->params['schemaDb'], $this->params['prefixoTabela'], $sufixo);
    }

    /**
     * zerando os registros associados à proposta
     *
     * @param $params
     */
    private function limparTabelas($params)
    {
        if ((int) $params['pagina'] === 0) {
            foreach ($this->mapa as $tabela) {
                $this->setTable($tabela);

                try {
                    $this->jobDelete([], true);
                } catch (Exception $e) {
                    throw $e;
                }
            }
        }
    }

    /**
     * @param $params
     * @param $retornoSiop
     * @param $paginacao
     *
     * @return array
     */
    private function isUltimaPagina($params, $retornoSiop, $paginacao)
    {
        if (count($retornoSiop->proposta) == $paginacao->registrosPorPagina) {
            return array_merge(
                $params
                , [
                    'terminate' => false,
                    'pagina'    => $params['pagina'] + 1
                ]
            );
        } else {
            return [
                'terminate' => true
            ];
        }
    }
}