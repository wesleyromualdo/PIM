<?php

/**
 * Classe de mapeamento da entidade par3.ensino_etapa.
 *
 * @version $Id$
 * @since 2017.05.08
 */

/**
 * Model_Ensino_etapa: sem descricao
 *
 * @package Model\Ensino
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Ensino_etapa($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Ensino_etapa($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $etaid Chave Primária - default: nextval('par3.ensino_etapa_etaid_seq'::regclass)
 * @property int $nivid Chave estrangeira que faz referência a tabela par3.ensino_nivel
 * @property string $etadsc Descrição da etapa do nível de Ensino
 */
class Par3_Model_EnsinoEtapa extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.ensino_etapa';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'etaid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'nivid' => array('tabela' => 'par3.ensino_nivel', 'pk' => 'nivid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'etaid' => null,
        'nivid' => null,
        'etadsc' => null,
    );

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function pegarSelectCombo(){
        $sql = "SELECT etaid as codigo,etadsc as descricao FROM {$this->stNomeTabela}";

        return $sql;
    }

    public function pegarSQLSelectCombo($nivid = array(), $etaid = array())
    {
        $where = array();
        $nivid = is_array($nivid) ? implode(',', $nivid) : $nivid;
        $etaid = is_array($etaid) ? implode(',', $etaid) : $etaid;
        if($nivid) $where[] = "nivid IN ($nivid)";
        if($etaid) $where[] = "etaid IN ($etaid)";

        $sql = "SELECT etaid as codigo,etadsc as descricao FROM {$this->stNomeTabela} WHERE ".implode(' AND ', $where);

        return $sql;
    }

    /**
     * Lista as etapas de ensino de acordo com o nível.
     *
     * @param $nivid
     * @return array|mixed|NULL
     */
    public function listaEnsinoEtapas($nivid)
    {
        $sql = "SELECT etaid as codigo,  etadsc as descricao
                FROM {$this->stNomeTabela}
                WHERE nivid = $nivid ";

        $dados = $this->carregar($sql);

        return $dados;
    }

    /**
     * @author Leo Kenzley <leo.oliveira@castgroup.com.br>
     * @param null $nivid
     * @param null $etaid
     * @return string
     */
    public function getFormListaEnsinoEtapa($nivid = null, $etaid = null){
        $sql = "";
        $where = '';
        $WHERE_EXISTES = 0;
        if(is_null($nivid) && is_null($etaid)){
            $sql  = "select etaid as codigo, etadsc as descricao from par3.ensino_etapa order by etadsc asc";
        }

        if(!is_null($nivid) || !is_null($etaid)){
            if(!is_null($nivid)){
                if(!is_null($etaid)){
                    $where .= ' AND ';
                }
            }

            if(!is_null($etaid)){
                $where .= " estaid = {$nivid} ";
            }
        }

        if($where != ''){
            $sql  = "select etaid as codigo, etadsc as descricao from par3.ensino_etapa WHERE {$where} order by etadsc asc";
        }

        return $sql;

    }

    /**
     * @param null $etaid
     */
    public function findById($etaid){
        if(!is_null($etaid)){
            $sql = "select * from par3.ensino_etapa as enta where eta.etaid = {$etaid}";
            try{
                $rowSet = $this->pegaLinha($sql);
                return $rowSet;
            }catch(Exception $e){
                return $e->getMessage();
            }
        }
    }

}
