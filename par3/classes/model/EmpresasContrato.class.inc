<?php
/**
 * Classe de mapeamento da entidade par3.empresas_contrato.
 *
 * @version $Id$
 * @since 2017.03.02
 */

/**
 * Par3_Model_Empresas_contrato: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Eduardo Dunice Neto <eduardo.neto@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Empresas_contrato($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Empresas_contrato($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $ecoid chave primária da empresa - default: nextval('par3.empresas_contrato_ecoid_seq'::regclass)
 * @property int $conid fk contrato
 * @property string $ecocnpjempresa cnpj das empresas
 * @property string $economeempresa nome das empresas
 * @property string $ecostatus 
 * @property \Datetime(Y-m-d H:i:s) $ecodtcriacao  - default: now()
 * @property string $usucpf 
 */
class Par3_Model_EmpresasContrato extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.empresas_contrato';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'ecoid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'conid' => array('tabela' => 'par3.contratos', 'pk' => 'conid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'ecoid' => null,
        'conid' => null,
        'ecocnpjempresa' => null,
        'economeempresa' => null,
        'ecostatus' => null,
        'ecodtcriacao' => null,
        'usucpf' => null,
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
            'ecoid' => array('Digits'),
            'conid' => array('allowEmpty' => true,'Digits'),
            'ecocnpjempresa' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 19))),
            'economeempresa' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'ecostatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'ecodtcriacao' => array('allowEmpty' => true),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
        );
    }

    public function montarSQLSimplesEmpresas($arrPost)
    {
        if($arrPost){
        }
        $sql = "SELECT ecoid as id,economeempresa,ecocnpjempresa,con.connumero
                FROM ".$this->stNomeTabela." as eco 
                INNER JOIN par3.contratos as con ON con.conid = eco.conid
                WHERE ecostatus = 'A'
                AND   con.inuid = {$arrPost['inuid']}
                ";
        return $sql;
    }

    public function verificarExisteEmpresaContratoCNPJ($arrPost)
    {
        if($arrPost['ecoid']){return false;}
        $this->ecocnpjempresa  = str_replace(array('.', '-', '/'), '', $arrPost['ecocnpjempresa']);
        $sql="SELECT * FROM $this->stNomeTabela as eco 
              WHERE eco.ecostatus = 'A'
              AND eco.ecocnpjempresa  = '{$this->ecocnpjempresa}'";
        $retorno = $this->pegaUm($sql);
        return $retorno;
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
