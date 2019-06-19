<?php
/**
 * Classe de mapeamento da entidade sase.contatoorganizacao.
 *
 * @version $Id$
 * @since 2017.06.02
 */

/**
 * Model_Contatoorganizacao: tabela guarda dados do cantato da organização
 *
 * @package Model\Contatoorganizacao
 * @uses Simec\Db\Modelo
 * @author  <>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Contatoorganizacao($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Contatoorganizacao($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property int $conid chave primaria da tabela
 * @property int $infterid chave estrageira da tabela sase.informacoesterritorio
 * @property string $contcpf CPF do contato da organização
 * @property string $connome nome do contato da organização
 * @property string $concargofuncao Cargo/Função contato da organização
 * @property string $conemail email contato da organização
 * @property string $contelefone1 telefone 1 contato da organização
 * @property string $comtelefone2 telefone 2 contato da organização
 * @property string $concep CEP contato da organização
 * @property string $conlogradouro logradouro contato da organização
 * @property string $concomplemento Complemento do endereço contato da organização
 * @property string $conbairro bairro do endereço contato da organização
 * @property string $concidade cidade  contato da organização
 * @property string $conuf UF contato da organização
 * @property string $consite site da organização
 * @property int $entid chafe estrangeira do tabela entidade.entidade
 */
class ContatoOrganizacao extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'sase.contatoorganizacao';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'conid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'entid' => array('tabela' => 'entidade.entidade', 'pk' => 'entid'),
        'infterid' => array('tabela' => 'sase.informacoesterritorio', 'pk' => 'infterid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'conid' => null,
        'infterid' => null,
        'contcpf' => null,
        'connome' => null,
        'concargofuncao' => null,
        'conemail' => null,
        'contelefone1' => null,
        'comtelefone2' => null,
        'concep' => null,
        'conlogradouro' => null,
        'concomplemento' => null,
        'conbairro' => null,
        'concidade' => null,
        'conuf' => null,
        'consite' => null,
        'entid' => null,
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
            'conid' => array('Digits'),
            'infterid' => array('allowEmpty' => true,'Digits'),
            'contcpf' => array(new Zend_Validate_StringLength(array('max' => 11))),
            'connome' => array(new Zend_Validate_StringLength(array('max' => 200))),
            'concargofuncao' => array(new Zend_Validate_StringLength(array('max' => 200))),
            'conemail' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 200))),
            'contelefone1' => array(new Zend_Validate_StringLength(array('max' => 15))),
            'comtelefone2' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 15))),
            'concep' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 9))),
            'conlogradouro' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'concomplemento' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'conbairro' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'concidade' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 100))),
            'conuf' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 2))),
            'consite' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'entid' => array('Digits'),
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

    function salvarContatoOrganizacao(){

        try{

            if (!empty($_POST['seceducacaoreferencial'])){
                $secEducacaoField = ',seceducacaoreferencial';
                $secEducacaoValue = ",{$_POST['seceducacaoreferencial']}";
            }

            $sql ="INSERT INTO sase.contatoorganizacao (
                                  infterid,
                                  contcpf,
                                  connome,
                                  concargofuncao,
                                  conemail,
                                  contelefone1,
                                  contelefone2,
                                  concep,
                                  conlogradouro,
                                  concomplemento,
                                  conbairro,
                                  concidade,
                                  conuf,
                                  consite
                                  $secEducacaoField)
                                VALUES (
                                   {$_POST[infterid]},
                                  '{$_POST[contcpf]}',
                                  '{$_POST[connome]}',
                                  '{$_POST[concargofuncao]}',
                                  '{$_POST[conemail]}',
                                  '{$_POST[contelefone1]}',
                                  '{$_POST[contelefone2]}',
                                  '{$_POST[concep]}',
                                  '{$_POST[conlogradouro]}',
                                  '{$_POST[concomplemento]}',
                                  '{$_POST[conbairro]}',
                                  '{$_POST[concidade]}',
                                  '{$_POST[conuf]}',
                                  '{$_POST[consite]}'
                                  $secEducacaoValue 
                                  )";
            $this->executar($sql);
            $this->commit();

        } catch ( Exception $e ) {
            $this->rollback();
            return false;
        }

        return true;
        
    }


    function editarContatoOrganizacao(){

        try{
            
            $sql ="
                    UPDATE sase.contatoorganizacao
                    SET 
                        contcpf = '{$_POST[contcpf]}', 
                        connome = '{$_POST[connome]}', 
                        concargofuncao = '{$_POST[concargofuncao]}',
                        conemail   = '{$_POST[conemail]}', 
                        contelefone1 = '{$_POST[contelefone1]}', 
                        contelefone2 = '{$_POST[contelefone2]}',
                        concep     = '{$_POST[concep]}', 
                        conlogradouro = '{$_POST[conlogradouro]}', 
                        concomplemento = '{$_POST[concomplemento]}', 
                        conbairro = '{$_POST[conbairro]}',
                        concidade  = '{$_POST[concidade]}', 
                        conuf = '{$_POST[conuf]}', 
                        consite = '{$_POST[consite]}', 
                        seceducacaoreferencial = {$_POST[seceducacaoreferencial]}
                    WHERE conid =   {$_POST[conid]}";
            $this->executar($sql);
            $this->commit();

        } catch ( Exception $e ) {
            $this->rollback();
            return false;
        }


        return true;

    }


}
