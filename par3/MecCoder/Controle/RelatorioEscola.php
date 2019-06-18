<?php
namespace Simec\Par3\Controle;

use Simec\Par3\Modelo\RelatorioEscola as RelatorioEscolaModel;

/**
 * Classe controladora responsável pela apresentação do relatório de escolas do módulo PAR3
 * @author calixto.jorge@gmail.com
 */
class RelatorioEscola extends \Simec\AbstractControle
{

    use \Simec\Par3\ControleTrait;

    public function finish()
    {
        
    }

    public function init()
    {
        
    }

    public function index()
    {
        $this->verTelaDePesquisa();
    }

    /**
     * Método visual de apresentação da tela de pesquisa de escolas
     */
    public function verTelaDePesquisa()
    {
        // Definições
        if ($this->session('par3_situacao_escola')) {
            $arrSituacao = $this->session('par3_situacao_escola');
        } else {
            $arrSituacao = (new RelatorioEscolaModel())->situacaoEscola();
            $this->session('par3_situacao_escola', $arrSituacao);
        }
        $responsabilidades = $this->usuarioResponsabilidade()->abrangencia();
        if (isset($responsabilidades['estados']) && count($responsabilidades['estados'])) {
            $estados = $responsabilidades['estados'];
        } else {
            $estados = (new \Territorios_Model_Estado())->pegarEstadosPorSigla();
        }
        $urlMaps = 'http://maps.google.com/maps/api/js?libraries=places&region=br'
            . '&language=pt-BR&key=AIzaSyCL34WyBqZOVNDu2DEa12qV3tr2JpNxkzs';
        // Registros
        $this->js('/zimec/public/temas/simec/js/funcoes.js');
        $this->js('/zimec/public/temas/simec/js/plugins/viewjs/view.js');
        $this->js('/zimec/public/temas/simec/js/plugins/zoomDetect.js');
        $this->js(IS_PRODUCAO ? GMAPS_API . 'libraries=places&region=br&language=pt-BR' : $urlMaps);
        $this->toJs('situacaoOpcoes', $arrSituacao);
        $this->toJs('planejamentoOpcoes', (new RelatorioEscolaModel())->opcoesPlanejamento());
        $this->toJs('monitoramentoOpcoes', (new RelatorioEscolaModel())->opcoesMonitoramento());
        $this->toJs('responsabilidade', $responsabilidades);
        $this->toJs('esferaOpcoes', (new RelatorioEscolaModel())->opcoesEsfera());
        $this->toJs('colunaOpcoes', (new RelatorioEscolaModel())->opcoesVisiveis());
        $this->toJs('estadoOpcoes', \Simec\Corporativo\Utilitario\Vetor::criarComNovosIndices($estados, ['codigo', 'descricao'], ['valor', 'descricao']
        ));
        // Apresentação
        $this->showHtml();
    }

    /**
     * Método de busca dos dados para a apresentação na listagem da tela de pesquisa de escolas
     */
    public function pesquisarDadosDaListagem()
    {
        $filtro = $this->post()['sfiltro'];
        $implode = function($ar) {
            $res = '';
            if (is_array($ar)) {
                foreach ($ar as $val) {
                    if ($val) {
                        $res .= "'{$val}',";
                    }
                }
            }
            return substr($res, 0, -1);
        };
        foreach (['estuf', 'esdid', 'mundescricao'] as $arArg) {
            $filtro[$arArg] = $implode($filtro[$arArg]);
        }
        if (isset($filtro['itrid'])) {
            $filtro['itrid'] = ['1' => 'Estadual', '2' => 'Municipal'][$filtro['itrid']];
        }
        if (isset($filtro['planejamento'])) {
            $filtro['planejamento'] = ['1' => 'possui', '2' => 'nao possui'][$filtro['planejamento']];
        }
        $this->{"formatarSaida{$this->post()['tipoSaida']}"}($filtro);
    }

    /**
     * Executa a formatação para a saída dos dados no formato de planilha
     * @param array $filtro
     */
    protected function formatarSaidaExcel($filtro)
    {
        $model = new RelatorioEscolaModel();
        $model->executarLeituraTodasEscolas($filtro);
        $this->showExcel(function()use($model) {
            return $model->pegarProximaEscola();
        });
    }

    /**
     * Executa a formatação para a saída dos dados no formato JSON
     * @param array $filtro
     */
    protected function formatarSaidaJson($filtro)
    {
        $model = new RelatorioEscolaModel();
        $arDadosEscolas = $model->lerPaginadoEscolas($filtro + $this->get());
        if(!$model->testa_superuser()) {
            unset($arDadosEscolas['sql']);
        }
        $this->showJson($arDadosEscolas);
    }

    /**
     * Realiza a leitura dos municípios 
     */
    public function lerMunicipios()
    {
        $resp = $this->usuarioResponsabilidade()->abrangencia();
        $model = new \Territorios_Model_Municipio();
        if (!(isset($resp['municipios']) && count($resp['municipios']))) {
            $resp['muncod'] = '';
        }
        $this->showJson($model->carregar($model->pegarSQLSelect($this->get()['estuf'], $resp['muncod'], true)));
    }

    /**
     * Realiza a leitura dos itens destinados para uma escola
     * $_POST['codInep']
     */
    public function lerItensDestinados()
    {
        $model = new RelatorioEscolaModel();
        $arDadosEscolas = $model->lerItensDestinados($this->post()['escid']);
        if(!$model->testa_superuser()) {
            unset($arDadosEscolas['sql']);
        }
        $this->showJson($arDadosEscolas);
    }

    public function detalheAssistencia()
    {
        $anaid = $this->post()['anaid'];
        $model = new \Par3_Model_IniciativaTiposAssistencia();
        $sql = $model->getAssistenciaPorAnaid($anaid);
        $arrDados = $model->pegaLinha($sql);
        $this->showJson($arrDados);
    }
}
