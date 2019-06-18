<?php
/**
 * Classe de mapeamento da entidade par3.ObraTipoItemPlanilhaValores.
 *
 * @version $Id$
 * @since 2017.07.05
 */

/**
 * Par3_Model_ObraTipoItemPlanilhaValores: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardoneto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_ObraTipoItemPlanilhaValores($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_ObraTipoItemPlanilhaValores($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $oivid  - default: nextval('par3.ObraTipoItemPlanilhaValores_oivid_seq'::regclass)
 * @property int $oipid
 * @property int $inuid
 * @property numeric $oivqtd
 * @property string $oivqtd_editavel
 * @property numeric $oivvalor
 * @property string $oivvalor_editavel
 */
class Par3_Model_ObraTipoItemPlanilhaValores extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_tipo_item_planilhaValores';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'oivid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inuid' => array('tabela' => 'par3.instrumentounidade', 'pk' => 'inuid'),
        'oipid' => array('tabela' => 'par3.obra_tipo_item_planilha', 'pk' => 'oipid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'oivid' => null,
        'oipid' => null,
        'inuid' => null,
        'oivqtd' => null,
        'oivqtd_editavel' => null,
        'oivvalor' => null,
        'oivvalor_editavel' => null,
        'oivstatus' => null,
    );

    //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function montarSQLSimples($arrPost)
    {
        $where = array("oiv.oivstatus = 'A'");
        if($arrPost['oipid']) $where[] = "oiv.oipid = '{$arrPost['oipid']}'";

        $orderBy = $arrPost['ordenacao'] ? "ORDER BY {$arrPost['ordenacao']['campo']} {$arrPost['ordenacao']['sentido']}":"";

        $sql = "SELECT
                	oivid||'_val' as id,
                    ROW_NUMBER() OVER ({$orderBy}) AS qtd,
                	oivid as codigo,
                	CASE WHEN inu.itrid = 2 THEN inu.estuf||' - ' ELSE '' END || coalesce(inudescricao, 'Nacional') as inudescricao,
                	oivqtd,
                	CASE WHEN oivqtd_editavel = 'S'
                	  THEN 'Sim'
                	  ELSE 'Não'
                	END as oivqtd_editavel,
                	oivvalor,
                	CASE WHEN oivvalor_editavel = 'S'
                	  THEN 'Sim'
                	  ELSE 'Não'
                	END as oivvalor_editavel
                FROM
                	par3.obra_tipo_item_planilha_valores oiv
                LEFT JOIN par3.instrumentounidade inu ON inu.inuid = oiv.inuid
                WHERE
                    ".implode(' AND ', $where)."
                ORDER BY
                	inudescricao";

        return $sql;
    }

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'oivid' => array('Digits'),
            'oipid' => array('allowEmpty' => true,'Digits'),
            'inuid' => array('allowEmpty' => true,'Digits'),
            'oivqtd' => array('allowEmpty' => true),
            'oivqtd_editavel' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'oivvalor' => array('allowEmpty' => true),
            'oivvalor_editavel' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 10))),
        );
    }

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

}
