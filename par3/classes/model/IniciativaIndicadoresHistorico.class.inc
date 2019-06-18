<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_indicadores_historico.
 *
 * @version $Id$
 * @since 2017.06.27
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_IniciativaIndicadoresHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Philipe Allan Almeida <philipe.almeida@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_IniciativaIndicadoresHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_IniciativaIndicadoresHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hidid chave primária da tabelainiciativa_indicadores_historico
 * @property  $hidcpf CPF do respnsável pela alteração
 * @property  $hidacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hiddtcriacao Data de criação do log
 * @property int $idiid Chave Primária - default: nextval('par3.iniciativa_indicadores_idiid_seq'::regclass)
 * @property int $iniid FK da Iniciativa (par3.iniciativa)
 * @property int $dimid FK da Dimensão (par3.dimensao)
 * @property int $areid FK da Área (par3.area)
 * @property int $indid FK do Indicador (par3.indicador)
 * @property int $crtid FK do Componentes do Indicador (par3.criterio)
 */
class Par3_Model_IniciativaIndicadoresHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_indicadores_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hidid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'idiid' => array('tabela' => 'par3.iniciativa_indicadores', 'pk' => 'idiid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hidid' => null,
        'hidcpf' => null,
        'hidacao' => null,
        'hiddtcriacao' => null,
        'idiid' => null,
        'iniid' => null,
        'dimid' => null,
        'areid' => null,
        'indid' => null,
        'crtid' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaIndicadores $model,$acao)
    {
        $this->arAtributos['hidcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hidacao'] = $acao;
        $this->arAtributos['hiddtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['idiid'] = $model->idiid;
        $this->arAtributos['iniid'] = $model->iniid;
        $this->arAtributos['dimid'] = $model->dimid;
        $this->arAtributos['areid'] = $model->areid;
        $this->arAtributos['indid'] = $model->indid;
        $this->arAtributos['crtid'] = $model->crtid;

        if ($acao == 'CREATE'){
            $this->arAtributos['idistatus'] = 'A';
        }
        if ($acao == 'DELETE'){
            $this->arAtributos['idistatus'] = 'I';
        }

        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $camposNull = array();
        //Faz o tratamento dos campos em branco
        foreach ($_POST as $chave => $valor) {
            if ($valor == '' && $chave) {
                $dados[$chave] = null;
                $camposNull[] = $chave;
            }
        }
        $this->salvar($boAntesSalvar = true, $boDepoisSalvar = true, $camposNull);
        $this->commit();
    }
}
