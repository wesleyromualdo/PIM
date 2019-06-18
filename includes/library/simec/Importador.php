<?php
/**
 * Created by PhpStorm.
 * User: victor
 * Date: 05/07/17
 * Time: 11:33
 */

class Importador
{

    /**
     * @var $model - Model utilizada pela importação
     */
    protected $model;

    /**
     * @var $campos - Campos utilizados na importação na ordem correta.
     */
    protected $campos;

    protected $boAntesSalvar = true;
    protected $boDepoisSalvar = true;
    protected $camposNullos = [];
    protected $manterAspas = false;
    protected $validador;
    protected $arquivoAberto;
    protected $offset;

    /**
     * @var $arquivo - Arquivo utilizado na importação
     */
    protected $arquivo;

    public function __construct ($model = null)
    {
        $this->setModel($model);
    }

    /**
     * Carrega a model desejada na classe
     *
     * @param $model
     *
     * @return $this
     */
    public function setModel ($model)
    {
        $this->model = $model;

        return $this;
    }

    /**
     * Retorna a model utilizada
     *
     * @return mixed
     */
    public function getModel ()
    {
        return $this->model;
    }

    /**
     * Retorna os atributos da model
     *
     * @return mixed
     */
    public function getDadosModel ()
    {
        return $this->model->getDados();
    }

    public function setValidador ($validador)
    {
        $this->validador = $validador;

        return $this;
    }

    /**
     * Seta os campos utilizados na importação, na ordem desejada
     *
     * @param array $campos
     *
     * @return $this
     */
    public function setCampos ($campos = [])
    {
        $this->campos = $campos;

        return $this;
    }

    /**
     * Retorna os campos utilizados na importação
     *
     * @return mixed
     */
    public function getCampos ()
    {
        return $this->campos;
    }

    public function setCamposNullos ($campos = [])
    {
        $this->camposNullos = $campos;

        return $this;
    }

    public function getCamposNullos ()
    {
        return $this->camposNullos;
    }

    public function setBoAntesSalvar ($bo = true)
    {
        $this->boAntesSalvar = $bo;

        return $this;
    }

    public function setBoDepoisSalvar ($bo = true)
    {
        $this->boDepoisSalvar = $bo;

        return $this;
    }

    public function setManterAspas ($bo = false)
    {
        $this->manterAspas = $bo;

        return $this;
    }

    public function setArquivo ($arquivo)
    {
        $this->arquivo = $arquivo;

        return $this;
    }

    public function setOffSet($offset) {
        $this->offset = $offset;
        return $this;
    }

    public function carregaArquivoCSV ($separador = ';', $validaLinha = true)
    {
        $this->arquivoAberto = fopen($this->arquivo, 'r');
        try
        {
            $i = 0;
            while ($linha = fgetcsv($this->arquivoAberto, null, $separador))
            {
                if ($this->offset && $i >= $this->offset) {
                    if ($i > 0) {
                        yield $linha;
                    } else {
                        if ($validaLinha) {
                            $this->validaLinhaCSV($linha);
                        }
                    }
                }
                $i++;
            }
        } finally
        {
            fclose($this->arquivoAberto);
        }
    }
    //TODO-renomear o metodo
    //TODO-finalizar quando a linha estiver vazia

    public function validaInportacao ()
    {
        if (empty($this->model))
        {
            throw new Exception('Model não carregada');
        }

        if (empty($this->campos))
        {
            throw new Exception('Campos não informados');
        }

        if (empty($this->arquivo))
        {
            throw new Exception('Arquivo não informado');
        }
    }

    public function importaCSV ()
    {
        $this->validaInportacao();
        $model = null;
        foreach ($this->carregaArquivoCSV() as $linha)
        {
            $model = new $this->model();
            foreach ($linha as $key => $valor)
            {
                $atributo = $this->campos[$key];
                $model->$atributo = $valor;
            }
            $model->salvar($this->boAntesSalvar, $this->boDepoisSalvar, $this->camposNullos, $this->manterAspas);
        }
        if (!$model->commit())
        {
            throw new Exception('Ocorreu um problema ao carregar o arquivo');
        }
    }

    public function closeArquivo ()
    {
        if (is_resource($this->arquivoAberto))
        {
            fclose($this->arquivoAberto);
        }
    }

    protected function validaLinhaCSV ($linha)
    {
        if (count($linha) != count($this->campos))
        {
            throw new Exception('Arquivo CSV não condiz com os campos informados.');
        }
    }
}