<?php
namespace Simec\Par3\Modelo\Iniciativa;

use Simec\Par3\Dado\Iniciativa\Iniciativa as dadosIniciativa;
use \Par3_Model_Iniciativa as modeloLegado;

class Iniciativa extends \Simec\AbstractModelo
{
    /**
     * @var dadosIniciativa
     */
    public $dados;

    protected $modeloLegado;

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        if ($id) $this->carregarDados($id, $tempocache);
    }

    public function carregarDados($id, $tempocache = null){
        if ( is_null($this->modeloLegado) ) $this->modeloLegado = new modeloLegado($id, $tempocache);
        $this->dados->carregar($this->modeloLegado->getArAtributos());
        return $this;
    }

    public function getIniciativa(array $arrPost)
    {
        return $this->modeloLegado->getIniciativa($arrPost);
    }

    public function sqlAnosIniciativaCombo($iniid)
    {
        return $this->modeloLegado->sqlAnosIniciativaCombo($iniid);
    }

    public function listarSelectDimensoesIniciativas($arrPost = null, $consulta = false)
    {
        return $this->modeloLegado->listarSelectDimensoesIniciativas($arrPost, $consulta);
    }

    public function listarSelectDimensoesIniciativasInp($arrPost = null, $consulta = false)
    {
        return $this->modeloLegado->listarSelectDimensoesIniciativasInp($arrPost, $consulta);
    }

    public function pegarIniciativa($arrPost)
    {
        return $this->modeloLegado->pegarIniciativa($arrPost);
    }

    public function getCamposValidacao($dados = array())
    {
        return $this->modeloLegado->getCamposValidacao($dados);
    }

    public function antesSalvar()
    {
        return $this->modeloLegado->antesSalvar();
    }

    public function verificarRestauracao($campos)
    {
        return $this->modeloLegado->verificarRestauracao($campos);
    }

    public function validaInclusaoIniciativaDetalhe(array $campos, $opcoes = array())
    {
        return $this->modeloLegado->validaInclusaoIniciativaDetalhe($campos, $opcoes);
    }

    public function validaInclusao(array $campos, $opcoes = array())
    {
        return $this->modeloLegado->validaInclusao($campos, $opcoes);
    }

    public function validarInput(array $campos, $opcoes = array())
    {
        return $this->modeloLegado->validarInput($campos, $opcoes);
    }

    public function verificaExistencia($campos, $opcoes = array())
    {
        return $this->modeloLegado->verificaExistencia($campos, $opcoes);
    }

    public function montarSQLSimples($arrPost)
    {
        return $this->modeloLegado->montarSQLSimples($arrPost);
    }

    public function recuperarProgramas($dados)
    {
        return $this->modeloLegado->recuperar($dados);
    }

    public function recuperarIniciativasPrograma($dados)
    {
        return $this->modeloLegado->recuperarIniciativasPrograma($dados);
    }

    public function recuperarIniciativa($dados)
    {
        return $this->modeloLegado->recuperarIniciativa($dados);
    }

    public function recuperarItensPospostaIniciativa($dados)
    {
        return $this->modeloLegado->recuperarItensPospostaIniciativa($dados);
    }

    public function recuperarItensIniciativa($dados)
    {
        return $this->modeloLegado->recuperarItensIniciativa($dados);
    }

    public function recuperarItensIniciativaEscola($dados)
    {
        return $this->modeloLegado->recuperarItensIniciativaEscola($dados);
    }

    public function recuperarQtdItensIniciativaEscola($dados)
    {
        return $this->modeloLegado->recuperarQtdItensIniciativaEscola($dados);
    }

    public function criaIniciativa($dados)
    {
        return $this->modeloLegado->criaIniciativa($dados);
    }

    public function criaIniciativaDetalhe($dados)
    {
        return $this->modeloLegado->criaIniciativaDetalhe($dados);
    }

    public function buscaItemComposicao($dados)
    {
        return $this->modeloLegado->buscaItemComposicao($dados);
    }

    public function inativaItemComposicao($icoid)
    {
        return $this->modeloLegado->inativaItemComposicao($icoid);
    }

    public function ativaItemComposicao($icoid)
    {
        return $this->modeloLegado->ativaItemComposicao($icoid);
    }

    public function insereItemComposicao($dados)
    {
        return $this->modeloLegado->insereItemComposicao($dados);
    }

    public function salvaQuantidadeItemComposicao($icoid, $icoquantidade)
    {
        return $this->modeloLegado->salvaQuantidadeItemComposicao($icoid, $icoquantidade);
    }

    public function salvaQuantidadeItemComposicaoEscola($sesid, $icoid, $icoquantidade)
    {
        return $this->modeloLegado->salvaQuantidadeItemComposicaoEscola($sesid, $icoid, $icoquantidade);
    }

    public function montaSQLPI($dados)
    {
        return $this->modeloLegado->montaSQLPI($dados);
    }

    public function montaSQLPTRES($dados)
    {
        return $this->modeloLegado->montaSQLPTRES($dados);
    }

    public function salvaIniciativaDetalhe($dados)
    {
        return $this->modeloLegado->salvaIniciativaDetalhe($dados);
    }

    public function recuperarEscolasMunicipio($objUnidade, $dados)
    {
        return $this->modeloLegado->recuperarEscolasMunicipio($objUnidade, $dados);
    }

    public function recuperarEscolasIniciativa($dados)
    {
        return $this->modeloLegado->recuperarEscolasIniciativa($dados);
    }

    public function aprovarIniciativas($dados)
    {
        return $this->modeloLegado->aprovarIniciativas($dados);
    }

    public function reprovarIniciativas($dados)
    {
        return $this->modeloLegado->reprovarIniciativas($dados);
    }

    public function getDadosIniciativaDadosDemograficos(array $arrPost)
    {
        return $this->modeloLegado->getDadosIniciativaDadosDemograficos($arrPost);
    }

    public function retornaEsferaIniciativa(array $arrPost)
    {
        return $this->modeloLegado->retornaEsferaIniciativa($arrPost);
    }

    public function getIniciativaById($iniid)
    {
        return $this->modeloLegado->getIniciativaById($iniid);
    }

    public function getIniciativaProinfancia(){
        return $this->modeloLegado->getIniciativaProinfancia();
    }

    public function getSqlRelatorioGerencial($request)
    {
        return $this->modeloLegado->getSqlRelatorioGerencial($request);
    }

    public function getSqlTotalizadorRelatorioGerencial( $request, $sqlPrincipal)
    {
        return $this->modeloLegado->getSqlTotalizadorRelatorioGerencial($request, $sqlPrincipal);
    }

    function getDadosSolicitacaoMobiliario($itrid){
        return $this->modeloLegado->getDadosSolicitacaoMobiliario($itrid);
    }

    function verificaIniciativaProinfancia(){
        return $this->modeloLegado->verificaIniciativaProinfancia();
    }

}
