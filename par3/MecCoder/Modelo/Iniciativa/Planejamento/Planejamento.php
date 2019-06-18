<?php
namespace Simec\Par3\Modelo\Iniciativa\Planejamento;

use Simec\ModeloLegadoTrait;
use Simec\Par3\Dado\Iniciativa\Planejamento\Planejamento as dadosIniciativaPlanejamento;

class Planejamento extends \Simec\AbstractModelo {

    use ModeloLegadoTrait;

    /**
     * @var dadosIniciativaPlanejamento
     */
    public  $dados;

    private $nomeModeloLegado = \Par3_Model_IniciativaPlanejamento::class;

    public function __construct($id = null, $tempocache = null)
    {
        parent::__construct();
        $this->carregarModeloLegado($this->nomeModeloLegado, $id, $tempocache);
    }

    public function retornaCategoriaIniciativa()
    {
        return $this->modeloLegado->retornaCategoriaIniciativa();
    }

    public function validarInput($campos)
    {
        return $this->modeloLegado->validarInput($campos);
    }

    public function montarSQLDataGrid($arrPost)
    {
        return $this->modeloLegado->montarSQLDataGrid($arrPost);
    }

    public function montarSqlDimensao($arrPost)
    {
        return $this->modeloLegado->montarSqlDimensao($arrPost);
    }


    public function montarSqlDimensaoImpressao($arrPost)
    {
        return $this->modeloLegado->montarSqlDimensaoImpressao($arrPost);
    }

    public function recuperarTodosEmCadastramento($inuid)
    {
        return $this->modeloLegado->recuperarTodosEmCadastramento($inuid);
    }

    public function recuperarObrasEmCadastramento($inuid, $inpid)
    {
        return $this->modeloLegado->recuperarObrasEmCadastramento($inuid, $inpid);
    }

    public function recuperarTodasFinalizadas($inuid)
    {
        return $this->modeloLegado->recuperarTodasFinalizadas($inuid);
    }

    public function montarSqlIniciativasDimensao($dimcod, $arrPost)
    {
        return $this->modeloLegado->montarSqlIniciativasDimensao($dimcod, $arrPost);
    }

    public function recuperaDocumento()
    {
        return $this->modeloLegado->recuperarDocumento();
    }

    public function recuperarTipoObra()
    {
        return $this->modeloLegado->recuperarTipoObra();
    }

    public function getIniciativaPlanejamentoByIdIniciativaDescricao($indid)
    {
        return $this->modeloLegado->getIniciativaPlanejamentoByIdIniciativaDescricao($indid);
    }

    public function recuperarPorIniciativa($iniid)
    {
        return $this->modeloLegado->recuperarPorIniciativa($iniid);
    }

    public function verificaExistenciaParaSalvar(array $array)
    {
        return $this->modeloLegado->verificaExistenciaParaSalvar($array);
    }

    public function verificaExistenciaPendenciasPreenchimento(array $array)
    {
        return $this->modeloLegado->verificaExistenciaPendenciasPreenchimento($array);
    }

    public function existeIniciativaJaCadastrada($inuid)
    {
        return $this->modeloLegado->existeIniciativaJaCadastrada($inuid);
    }

    public function getSituacaoIniciativa($filtro = '')
    {
        return $this->modeloLegado->getSituacaoIniciativa($filtro);
    }

    public function getSituacaoAnalise()
    {
        return $this->modeloLegado->getSituacaoAnalise();
    }

    public function getTecnicoAnalise()
    {
        return $this->modeloLegado->getTecnicoAnalise();
    }

    public function getInicitivaAno()
    {
        return $this->modeloLegado->getIniciativaAno();
    }

    public function getTipos_Assistencia()
    {
        return $this->modeloLegado->getTipos_Assistencia();
    }

    public function getValorTotalPlanejamento($arrPost)
    {
        return $this->modeloLegado->getValorTotalPlanejamento($arrPost);
    }

    public function getEstadoMunicipioPorIniciativa($municipio = false)
    {
        return $this->modeloLegado->getEstadoMunicipioPorIniciativa($municipio);
    }

    public function acessoRapidoCarregarComboMunicipio($arParam=array(), $boolSql=true)
    {
        return $this->modeloLegado->acessoRapidoCarregarComboMunicipio($arParam, $boolSql);
    }

    public function getTipoObjetoporIniciativa($and = array())
    {
        return $this->modeloLegado->getTipoObjetoporIniciativa($and);
    }

    public function getTipoAtendimentoIniciativa()
    {
        return $this->modeloLegado->getTipoAtendimentoIniciativa();
    }

    public function getProgramaIniciativa()
    {
        return $this->modeloLegado->getProgramaIniciativa();
    }

    public function getEtapaIniciativa()
    {
        return $this->modeloLegado->getEtapaIniciativa();
    }

    public function getDesdobramentoIniciativa()
    {
        return $this->modeloLegado->getDesdobramentoIniciativa();
    }

    public function getEscolaIniciativa($muncod = null)
    {
        return $this->modeloLegado->getEscolaIniciativa($muncod);
    }

    public function verificarExisteIniciativa($inuid)
    {
        return $this->modeloLegado->verificarExisteIniciativa($inuid);
    }

    public function verificarSituacaoIniciativa($arrPost)
    {
        return $this->modeloLegado->verificarSituacaoIniciativa($arrPost);
    }

    public function getEstadosIniciativas()
    {
        $sql = "SELECT
					estadodocumento.esdid as codigo,
					estadodocumento.esddsc  as descricao
					FROM
					workflow.estadodocumento
					WHERE
					workflow.estadodocumento.tpdid =  '302'
                    AND workflow.estadodocumento.esdstatus = 'A'
					ORDER BY esdordem ASC;";

        return $this->carregar($sql);
    }

    public function sqlListaDocumentoPlanejamento($inpid)
    {
        return $this->modeloLegado->sqlListaDocumentoPlanejamento($inpid);
    }

    public function salvarDocumentoPlanejamento($inpid, $arqid, $dsc)
    {
        return $this->modeloLegado->salvarDocumentoPlanejamento($inpid, $arqid, $dsc);
    }

    public function removerDocumentoPlanejamento($arqid)
    {
        return $this->modeloLegado->removerDocumentoPlanejamento($arqid);
    }

    public function getDimensaoIniciativas($inuid)
    {
        return $this->modeloLegado->getDimensaoIniciativas($inuid);
    }

    public function getDimensaoIniciativasPlanejamento($inuid, $dimid)
    {
        return $this->modeloLegado->getDimensaoIniciativasPlanejamento($inuid, $dimid);
    }

    public function recuperaIniciativaProinfancia($obrid)
    {
        return $this->modeloLegado->recuperaIniciativaProinfancia($obrid);
    }

}