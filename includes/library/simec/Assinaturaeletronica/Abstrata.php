<?php
/**
 * Classe de mapeamento da entidade seguranca.assinaturaeletronica.
 *
 * @version $Id$
 * @since 2017.10.09
 */

require_once APPRAIZ . 'includes/classes/Modelo.class.inc';

/**
 * Simec_Model_Assinaturaeletronica: Tabela para controle de assinatura eletrônica de documentos
 *
 * @package Simec\Model
 * @uses Simec\Db\Modelo
 * @author Tiago GalvÃo Mascarenhas Freire <tiagofreire@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Simec_Model_Assinaturaeletronica($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Simec_Model_Assinaturaeletronica($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $aseid Chave primária da tabela - default: nextval('seguranca.assinaturaeletronica_aseid_seq'::regclass)
 * @property string $usucpf Chave estrangeria da tabela de usuário com quem fez a assinatura
 * @property string $usucpfcancelamento Chave estrangeria da tabela de usuário com quem cancelou a assinatura
 * @property \Datetime(Y-m-d H:i:s) $asedata Data da assinatura - default: now()
 * @property \Datetime(Y-m-d H:i:s) $asedatacancelamento Data de cancelamento da assinatura
 * @property string $aseobscancelamento Texto de observação com as informações relevantes ao cancelamento da assinatura
 * @property string $asecargo Cargo do autor da assinatura
 * @property string $asecodigovalidacao Código único para validação da assinatura
 * @property string $asestatus Status do registro: A para Ativo e I para Inativo - default: 'A'::bpchar
 */
abstract class Simec_Assinaturaeletronica_Abstrata extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'seguranca.assinaturaeletronica';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'aseid',
        'asecodigovalidacao',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'usucpfcancelamento' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
        'usucpf' => array('tabela' => 'seguranca.usuario', 'pk' => 'usucpf'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'aseid' => null,
        'usucpf' => null,
        'usucpfcancelamento' => null,
        'asedata' => null,
        'asedatacancelamento' => null,
        'aseobscancelamento' => null,
        'asecargo' => null,
        'asecodigovalidacao' => null,
        'asestatus' => null,
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
            'aseid' => array('Digits'),
            'usucpf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'usucpfcancelamento' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 11))),
            'asedata' => array('allowEmpty' => true),
            'asedatacancelamento' => array('allowEmpty' => true),
            'aseobscancelamento' => array('allowEmpty' => true),
            'asecargo' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 200))),
            'asecodigovalidacao' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 20))),
            'asestatus' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
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
