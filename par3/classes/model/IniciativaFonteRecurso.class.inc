<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_fonte_recurso.
 *
 * @version $Id$
 * @since 2017.10.09
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Par3_Model_Iniciativa_fonte_recurso: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Hemerson Expedito Dornelas Morais <hemerson.moraiss@gmail.com>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_fonte_recurso($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_fonte_recurso($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $ifrusucpfinativacao
 * @property \Datetime(Y-m-d H:i:s) $ifrdtinativacao
 * @property string $ifrstatus  - default: 'A'::bpchar
 * @property string $ifrdescicaofonte
 * @property string $ifrcodigofonte
 * @property int $inplintid
 * @property int $ifrid  - default: nextval('par3.iniciativa_fonte_recurso_ifrid_seq'::regclass)
 */
class Par3_Model_IniciativaFonteRecurso extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_fonte_recurso';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ifrid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'inplintid' => array('tabela' => 'par3.iniciativa_plano_interno', 'pk' => 'inplintid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ifrusucpfinativacao' => null,
        'ifrdtinativacao' => null,
        'ifrstatus' => null,
        'ifrdescicaofonte' => null,
        'ifrcodigofonte' => null,
        'inplintid' => null,
        'ifrid' => null,
        'ifrano' => null,
        'ifrsituacao'=>null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'ifrusucpfinativacao' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 11))),
            'ifrdtinativacao' => array('allowEmpty' => true),
            'ifrstatus' => array(new Zend_Validate_StringLength(array('max' => 1))),
            'ifrdescicaofonte' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'ifrcodigofonte' => array('allowEmpty' => true, new Zend_Validate_StringLength(array('max' => 255))),
            'inplintid' => array('Digits'),
            'ifrid' => array('Digits'),
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


    public function montarSQLSimples($arrPost){


        $where = $arrPost['inplintid'] && validarInteiro($arrPost['inplintid']) ? " AND ifr.inplintid in({$arrPost['inplintid']})" : '';
        $where .= $arrPost['ifrcodigofonte'] ? " AND ifr.ifrcodigofonte ILIKE upper(removeacento('%{$arrPost['ifrcodigofonte']}%'))" : '';
        $where .= $arrPost['ifrdescicaofonte'] ? " AND ifr.ifrdescicaofonte ILIKE upper(removeacento('%{$arrPost['ifrdescicaofonte']}%'))" : '';
        $where .= $arrPost['ifrano'] && validarInteiro($arrPost['ifrano']) ? " AND ifr.ifrano in({$arrPost['ifrano']})" : '';



        $sql = "
     
              SELECT 
				  ifr.ifrid,
				  ifr.ifrid AS acao,
				  ipi.inplintdsc,
				  ipi.inplintptres,
				  ifr.ifrcodigofonte,
				  ifr.ifrdescicaofonte,
				  ifr.ifrano,
				  ifr.ifrsituacao
            FROM
                par3.iniciativa_fonte_recurso ifr
                INNER JOIN par3.iniciativa_plano_interno ipi ON ipi.inplintid = ifr.inplintid
                WHERE ifrstatus ='A'
  
                    {$where}
                   ORDER BY ifr.ifrid desc
          
            ";
  // ver($sql);
        return $sql;

    }



}
